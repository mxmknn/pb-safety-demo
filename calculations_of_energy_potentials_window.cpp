#include "calculations_of_energy_potentials_window.h"

#include <QAbstractItemView>
#include <QFont>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QVariant>
#include <QVBoxLayout>
#include <QWidget>

namespace {
constexpr auto kMainConnectionName = "pb_safety_main";

QString boolToYesNo(bool value)
{
    return value ? QStringLiteral("Да") : QStringLiteral("Нет");
}

QString notCalculated()
{
    return QStringLiteral("-");
}
}

Calculations_Energy_window::Calculations_Energy_window(QWidget *parent)
    : QMainWindow(parent)
{
    buildInterface();
    setAttribute(Qt::WA_DeleteOnClose);
    refreshUnits();
}

QPushButton *Calculations_Energy_window::createButton(const QString &text, int width)
{
    QPushButton *button = new QPushButton(text, this);
    button->setMinimumWidth(width);
    button->setFixedHeight(32);
    return button;
}

void Calculations_Energy_window::createUnitsTable()
{
    unitsTable = new QTableView(this);
    unitsModel = new QStandardItemModel(0, UnitColumnCount, unitsTable);

    unitsModel->setHeaderData(UnitNameColumn, Qt::Horizontal, "Название установки");
    unitsModel->setHeaderData(UnitCommentColumn, Qt::Horizontal, "Комментарий");

    unitsTable->setModel(unitsModel);
    unitsTable->verticalHeader()->setVisible(false);
    unitsTable->horizontalHeader()->setStretchLastSection(true);
    unitsTable->horizontalHeader()->setFixedHeight(26);
    unitsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    unitsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    unitsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    unitsTable->setFocusPolicy(Qt::StrongFocus);
    unitsTable->setColumnWidth(UnitNameColumn, 520);
    unitsTable->setColumnWidth(UnitCommentColumn, 420);
    unitsTable->setFixedHeight(190);

    connect(unitsTable->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &Calculations_Energy_window::onUnitsSelectionChanged);
}

void Calculations_Energy_window::createBlocksTable()
{
    blocksTable = new QTableView(this);
    blocksModel = new QStandardItemModel(0, BlockColumnCount, blocksTable);

    blocksModel->setHeaderData(BlockNameColumn, Qt::Horizontal, "Название блока");
    blocksModel->setHeaderData(BlockKEruptionColumn, Qt::Horizontal, "Коэф.\nуч. во\nвзрыве");
    blocksModel->setHeaderData(BlockClassColumn, Qt::Horizontal, "Токсичные,\nвысокотокс\nичные в-ва");
    blocksModel->setHeaderData(BlockCategoryColumn, Qt::Horizontal, "Кате-\nгория:");
    blocksModel->setHeaderData(BlockTotalEnergyColumn, Qt::Horizontal, "Полный\nпотен-циал");
    blocksModel->setHeaderData(BlockReducedMassColumn, Qt::Horizontal, "Приведен-\nная масса:");
    blocksModel->setHeaderData(BlockRelativePotentialColumn, Qt::Horizontal, "Относитель\nный\nпотенциал:");
    blocksModel->setHeaderData(BlockRadiusColumn, Qt::Horizontal, "Радиус\nRo:");
    blocksModel->setHeaderData(BlockPrintRColumn, Qt::Horizontal, "Оценка\nзон\nпораж.");

    blocksTable->setModel(blocksModel);
    blocksTable->verticalHeader()->setVisible(false);
    blocksTable->horizontalHeader()->setFixedHeight(78);
    blocksTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    blocksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    blocksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    blocksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    blocksTable->setFocusPolicy(Qt::StrongFocus);

    blocksTable->setColumnWidth(BlockNameColumn, 360);
    blocksTable->setColumnWidth(BlockKEruptionColumn, 70);
    blocksTable->setColumnWidth(BlockClassColumn, 95);
    blocksTable->setColumnWidth(BlockCategoryColumn, 70);
    blocksTable->setColumnWidth(BlockTotalEnergyColumn, 100);
    blocksTable->setColumnWidth(BlockReducedMassColumn, 100);
    blocksTable->setColumnWidth(BlockRelativePotentialColumn, 115);
    blocksTable->setColumnWidth(BlockRadiusColumn, 75);
    blocksTable->setColumnWidth(BlockPrintRColumn, 80);
    blocksTable->setFixedHeight(200);

    connect(blocksTable->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &Calculations_Energy_window::onBlocksSelectionChanged);
}

void Calculations_Energy_window::onUnitsSelectionChanged()
{
    refreshBlocks();
    updateActionStates();
}

void Calculations_Energy_window::onBlocksSelectionChanged()
{
    updateActionStates();
}

void Calculations_Energy_window::clearBlocks()
{
    blocksModel->removeRows(0, blocksModel->rowCount());
    blocksTable->clearSelection();
}

bool Calculations_Energy_window::ensureDatabaseReady(const QString &actionName) const
{
    const QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    if (db.isValid() && db.isOpen()) {
        return true;
    }

    QMessageBox::critical(const_cast<Calculations_Energy_window *>(this),
                          QStringLiteral("Ошибка базы данных"),
                          QStringLiteral("%1\n\nСоединение с базой данных неактивно.").arg(actionName));
    return false;
}

bool Calculations_Energy_window::executeQuery(QSqlQuery &query, const QString &actionName) const
{
    if (query.exec()) {
        return true;
    }

    QMessageBox::critical(const_cast<Calculations_Energy_window *>(this),
                          QStringLiteral("Ошибка базы данных"),
                          QStringLiteral("%1\n\n%2").arg(actionName, query.lastError().text()));
    return false;
}

bool Calculations_Energy_window::beginTransaction(const QString &actionName, QSqlDatabase &db) const
{
    if (db.transaction()) {
        return true;
    }

    QMessageBox::critical(const_cast<Calculations_Energy_window *>(this),
                          QStringLiteral("Ошибка базы данных"),
                          QStringLiteral("%1\n\nНе удалось начать транзакцию.").arg(actionName));
    return false;
}

bool Calculations_Energy_window::commitTransaction(const QString &actionName, QSqlDatabase &db) const
{
    if (db.commit()) {
        return true;
    }

    db.rollback();
    QMessageBox::critical(const_cast<Calculations_Energy_window *>(this),
                          QStringLiteral("Ошибка базы данных"),
                          QStringLiteral("%1\n\nНе удалось завершить транзакцию.").arg(actionName));
    return false;
}

void Calculations_Energy_window::rollbackTransaction(QSqlDatabase &db) const
{
    db.rollback();
}

int Calculations_Energy_window::findRowById(const QStandardItemModel *model, qint64 id) const
{
    if (!model || id < 0) {
        return -1;
    }

    for (int row = 0; row < model->rowCount(); ++row) {
        const QStandardItem *item = model->item(row, 0);
        if (item && item->data(IdRole).toLongLong() == id) {
            return row;
        }
    }

    return -1;
}

qint64 Calculations_Energy_window::selectedId(const QTableView *table, const QStandardItemModel *model) const
{
    if (!table || !model) {
        return -1;
    }

    const QModelIndex current = table->currentIndex();
    if (!current.isValid()) {
        return -1;
    }

    const QStandardItem *item = model->item(current.row(), 0);
    return item ? item->data(IdRole).toLongLong() : -1;
}

int Calculations_Energy_window::selectedIndexValue(const QTableView *table, const QStandardItemModel *model) const
{
    if (!table || !model) {
        return -1;
    }

    const QModelIndex current = table->currentIndex();
    if (!current.isValid()) {
        return -1;
    }

    const QStandardItem *item = model->item(current.row(), 0);
    return item ? item->data(IndexRole).toInt() : -1;
}

void Calculations_Energy_window::selectRow(QTableView *table, QStandardItemModel *model, int row) const
{
    if (!table || !model || row < 0 || row >= model->rowCount()) {
        return;
    }

    table->setCurrentIndex(model->index(row, 0));
}

qint64 Calculations_Energy_window::selectedUnitId() const
{
    return selectedId(unitsTable, unitsModel);
}

qint64 Calculations_Energy_window::selectedBlockId() const
{
    return selectedId(blocksTable, blocksModel);
}

int Calculations_Energy_window::selectedUnitIndexValue() const
{
    return selectedIndexValue(unitsTable, unitsModel);
}

int Calculations_Energy_window::selectedBlockIndexValue() const
{
    return selectedIndexValue(blocksTable, blocksModel);
}

bool Calculations_Energy_window::nextUnitIndex(QSqlDatabase &db, int *value) const
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT COALESCE(MAX([Index]), 0) + 1 FROM Unit;"));
    if (!executeQuery(query, QStringLiteral("Не удалось вычислить следующий индекс установки"))) {
        return false;
    }

    if (!query.next()) {
        QMessageBox::critical(const_cast<Calculations_Energy_window *>(this),
                              QStringLiteral("Ошибка базы данных"),
                              QStringLiteral("Не удалось прочитать следующий индекс установки."));
        return false;
    }

    *value = query.value(0).toInt();
    return true;
}

bool Calculations_Energy_window::nextBlockIndex(QSqlDatabase &db, qint64 unitId, int *value) const
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT COALESCE(MAX([Index]), 0) + 1 FROM Block WHERE ID_Unit = ?;"));
    query.addBindValue(unitId);
    if (!executeQuery(query, QStringLiteral("Не удалось вычислить следующий индекс блока"))) {
        return false;
    }

    if (!query.next()) {
        QMessageBox::critical(const_cast<Calculations_Energy_window *>(this),
                              QStringLiteral("Ошибка базы данных"),
                              QStringLiteral("Не удалось прочитать следующий индекс блока."));
        return false;
    }

    *value = query.value(0).toInt();
    return true;
}

bool Calculations_Energy_window::swapIndexes(const QString &tableName,
                                              qint64 currentId,
                                              int currentIndex,
                                              qint64 neighborId,
                                              int neighborIndex,
                                              const QString &actionName)
{
    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    if (!beginTransaction(actionName, db)) {
        return false;
    }

    QSqlQuery updateCurrent(db);
    updateCurrent.prepare(QStringLiteral("UPDATE %1 SET [Index] = ? WHERE ID = ?;").arg(tableName));
    updateCurrent.addBindValue(neighborIndex);
    updateCurrent.addBindValue(currentId);
    if (!executeQuery(updateCurrent, actionName)) {
        rollbackTransaction(db);
        return false;
    }

    QSqlQuery updateNeighbor(db);
    updateNeighbor.prepare(QStringLiteral("UPDATE %1 SET [Index] = ? WHERE ID = ?;").arg(tableName));
    updateNeighbor.addBindValue(currentIndex);
    updateNeighbor.addBindValue(neighborId);
    if (!executeQuery(updateNeighbor, actionName)) {
        rollbackTransaction(db);
        return false;
    }

    return commitTransaction(actionName, db);
}

void Calculations_Energy_window::refreshUnits(qint64 preferredUnitId)
{
    if (!ensureDatabaseReady(QStringLiteral("Не удалось обновить список установок"))) {
        unitsModel->removeRows(0, unitsModel->rowCount());
        clearBlocks();
        updateActionStates();
        return;
    }

    const qint64 unitIdToSelect = (preferredUnitId >= 0) ? preferredUnitId : selectedUnitId();

    unitsModel->removeRows(0, unitsModel->rowCount());

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    QSqlQuery query(db);

    if (showDeletedUnits) {
        query.prepare(QStringLiteral(
            "SELECT ID, iUnitName, iComment, [Index] "
            "FROM Unit WHERE [Index] < 0 "
            "ORDER BY DeleteDate DESC, ID DESC;"));
    } else {
        query.prepare(QStringLiteral(
            "SELECT ID, iUnitName, iComment, [Index] "
            "FROM Unit WHERE [Index] >= 0 "
            "ORDER BY [Index];"));
    }

    if (!executeQuery(query, QStringLiteral("Не удалось загрузить список установок"))) {
        clearBlocks();
        updateActionStates();
        return;
    }

    while (query.next()) {
        QStandardItem *nameItem = new QStandardItem(query.value(1).toString());
        QStandardItem *commentItem = new QStandardItem(query.value(2).toString());
        nameItem->setData(query.value(0).toLongLong(), IdRole);
        nameItem->setData(query.value(3).toInt(), IndexRole);
        unitsModel->appendRow({nameItem, commentItem});
    }

    int rowToSelect = findRowById(unitsModel, unitIdToSelect);
    if (rowToSelect < 0 && unitsModel->rowCount() > 0) {
        rowToSelect = 0;
    }

    {
        QSignalBlocker blocker(unitsTable->selectionModel());
        if (rowToSelect >= 0) {
            selectRow(unitsTable, unitsModel, rowToSelect);
        } else {
            unitsTable->clearSelection();
        }
    }

    if (unitDeletedButton) {
        unitDeletedButton->setText(showDeletedUnits ? QStringLiteral("Активные") : QStringLiteral("Удаленные"));
    }

    if (rowToSelect >= 0) {
        refreshBlocks();
    } else {
        clearBlocks();
    }

    updateActionStates();
}

void Calculations_Energy_window::refreshBlocks(qint64 preferredBlockId)
{
    blocksModel->removeRows(0, blocksModel->rowCount());

    const qint64 unitId = selectedUnitId();
    if (unitId < 0) {
        blocksTable->clearSelection();
        updateActionStates();
        return;
    }

    if (!ensureDatabaseReady(QStringLiteral("Не удалось обновить список блоков"))) {
        updateActionStates();
        return;
    }

    const qint64 blockIdToSelect = (preferredBlockId >= 0) ? preferredBlockId : selectedBlockId();

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT ID, iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, [Index] "
        "FROM Block WHERE ID_Unit = ? "
        "ORDER BY [Index];"));
    query.addBindValue(unitId);

    if (!executeQuery(query, QStringLiteral("Не удалось загрузить список блоков"))) {
        updateActionStates();
        return;
    }

    while (query.next()) {
        const qint64 id = query.value(0).toLongLong();
        const QString blockName = query.value(1).toString();
        const QString kEruption = query.value(2).isNull() ? notCalculated() : query.value(2).toString();
        const bool classOneTwo = query.value(3).toBool();
        const bool printRadius = query.value(4).toBool();
        const int indexValue = query.value(5).toInt();

        QStandardItem *nameItem = new QStandardItem(blockName);
        nameItem->setData(id, IdRole);
        nameItem->setData(indexValue, IndexRole);

        blocksModel->appendRow({nameItem,
                                new QStandardItem(kEruption),
                                new QStandardItem(boolToYesNo(classOneTwo)),
                                new QStandardItem(notCalculated()),
                                new QStandardItem(notCalculated()),
                                new QStandardItem(notCalculated()),
                                new QStandardItem(notCalculated()),
                                new QStandardItem(notCalculated()),
                                new QStandardItem(boolToYesNo(printRadius))});
    }

    int rowToSelect = findRowById(blocksModel, blockIdToSelect);
    if (rowToSelect < 0 && blocksModel->rowCount() > 0) {
        rowToSelect = 0;
    }

    {
        QSignalBlocker blocker(blocksTable->selectionModel());
        if (rowToSelect >= 0) {
            selectRow(blocksTable, blocksModel, rowToSelect);
        } else {
            blocksTable->clearSelection();
        }
    }

    updateActionStates();
}

void Calculations_Energy_window::updateActionStates()
{
    const bool hasUnit = selectedUnitId() >= 0;
    const bool hasBlock = selectedBlockId() >= 0;
    const bool allowEdits = !showDeletedUnits;

    if (unitAddButton) unitAddButton->setEnabled(allowEdits);
    if (unitDeleteButton) unitDeleteButton->setEnabled(allowEdits && hasUnit);
    if (unitCopyButton) unitCopyButton->setEnabled(allowEdits && hasUnit);
    if (unitUpButton) unitUpButton->setEnabled(allowEdits && hasUnit);
    if (unitDownButton) unitDownButton->setEnabled(allowEdits && hasUnit);

    if (blockAddButton) blockAddButton->setEnabled(allowEdits && hasUnit);
    if (blockDeleteButton) blockDeleteButton->setEnabled(allowEdits && hasBlock);
    if (blockCopyButton) blockCopyButton->setEnabled(allowEdits && hasBlock);
    if (blockUpButton) blockUpButton->setEnabled(allowEdits && hasBlock);
    if (blockDownButton) blockDownButton->setEnabled(allowEdits && hasBlock);
}

void Calculations_Energy_window::showNotImplemented(const QString &actionName)
{
    QMessageBox::information(this,
                             QStringLiteral("Функция в разработке"),
                             QStringLiteral("%1 будет реализовано на следующем этапе.").arg(actionName));
}

void Calculations_Energy_window::addUnit()
{
    if (!ensureDatabaseReady(QStringLiteral("Не удалось добавить установку"))) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    if (!beginTransaction(QStringLiteral("Не удалось добавить установку"), db)) {
        return;
    }

    int indexValue = 1;
    if (!nextUnitIndex(db, &indexValue)) {
        rollbackTransaction(db);
        return;
    }

    QSqlQuery insert(db);
    insert.prepare(QStringLiteral(
        "INSERT INTO Unit (iUnitName, iComment, [Index], DeleteDate) "
        "VALUES (?, ?, ?, NULL);"));
    insert.addBindValue(QStringLiteral("Новая установка"));
    insert.addBindValue(QString());
    insert.addBindValue(indexValue);

    if (!executeQuery(insert, QStringLiteral("Не удалось добавить установку"))) {
        rollbackTransaction(db);
        return;
    }

    if (!commitTransaction(QStringLiteral("Не удалось добавить установку"), db)) {
        return;
    }

    refreshUnits(insert.lastInsertId().toLongLong());
    showNotImplemented(QStringLiteral("Редактор установки"));
}

void Calculations_Energy_window::deleteSelectedUnit()
{
    const qint64 unitId = selectedUnitId();
    if (unitId < 0) {
        return;
    }

    if (!ensureDatabaseReady(QStringLiteral("Не удалось удалить установку"))) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    QSqlQuery query(db);
    query.prepare(QStringLiteral("UPDATE Unit SET [Index] = -1, DeleteDate = CURRENT_TIMESTAMP WHERE ID = ?;"));
    query.addBindValue(unitId);

    if (!executeQuery(query, QStringLiteral("Не удалось удалить установку"))) {
        return;
    }

    refreshUnits();
}

void Calculations_Energy_window::copySelectedUnit()
{
    const qint64 sourceUnitId = selectedUnitId();
    if (sourceUnitId < 0) {
        return;
    }

    if (!ensureDatabaseReady(QStringLiteral("Не удалось скопировать установку"))) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    if (!beginTransaction(QStringLiteral("Не удалось скопировать установку"), db)) {
        return;
    }

    QSqlQuery sourceUnit(db);
    sourceUnit.prepare(QStringLiteral(
        "SELECT iUnitName, iComment, iDocuments, iDiscription, TRIAL922 "
        "FROM Unit WHERE ID = ?;"));
    sourceUnit.addBindValue(sourceUnitId);

    if (!executeQuery(sourceUnit, QStringLiteral("Не удалось прочитать исходную установку")) || !sourceUnit.next()) {
        rollbackTransaction(db);
        return;
    }

    int unitIndex = 1;
    if (!nextUnitIndex(db, &unitIndex)) {
        rollbackTransaction(db);
        return;
    }

    QSqlQuery insertUnit(db);
    insertUnit.prepare(QStringLiteral(
        "INSERT INTO Unit (iUnitName, iComment, iDocuments, iDiscription, [Index], DeleteDate, TRIAL922) "
        "VALUES (?, ?, ?, ?, ?, NULL, ?);"));
    insertUnit.addBindValue(sourceUnit.value(0).toString() + QStringLiteral(" (копия)"));
    insertUnit.addBindValue(sourceUnit.value(1));
    insertUnit.addBindValue(sourceUnit.value(2));
    insertUnit.addBindValue(sourceUnit.value(3));
    insertUnit.addBindValue(unitIndex);
    insertUnit.addBindValue(sourceUnit.value(4));

    if (!executeQuery(insertUnit, QStringLiteral("Не удалось создать копию установки"))) {
        rollbackTransaction(db);
        return;
    }

    const qint64 newUnitId = insertUnit.lastInsertId().toLongLong();

    QSqlQuery sourceBlocks(db);
    sourceBlocks.prepare(QStringLiteral(
        "SELECT iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, iDerevoVer_ID, TRIAL922 "
        "FROM Block WHERE ID_Unit = ? "
        "ORDER BY [Index];"));
    sourceBlocks.addBindValue(sourceUnitId);

    if (!executeQuery(sourceBlocks, QStringLiteral("Не удалось прочитать блоки исходной установки"))) {
        rollbackTransaction(db);
        return;
    }

    int blockIndex = 1;
    if (!nextBlockIndex(db, newUnitId, &blockIndex)) {
        rollbackTransaction(db);
        return;
    }

    while (sourceBlocks.next()) {
        QSqlQuery insertBlock(db);
        insertBlock.prepare(QStringLiteral(
            "INSERT INTO Block "
            "(ID_Unit, iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, [Index], iDerevoVer_ID, TRIAL922) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"));
        insertBlock.addBindValue(newUnitId);
        insertBlock.addBindValue(sourceBlocks.value(0).toString() + QStringLiteral(" (копия)"));
        insertBlock.addBindValue(sourceBlocks.value(1));
        insertBlock.addBindValue(sourceBlocks.value(2));
        insertBlock.addBindValue(sourceBlocks.value(3));
        insertBlock.addBindValue(sourceBlocks.value(4));
        insertBlock.addBindValue(blockIndex++);
        insertBlock.addBindValue(sourceBlocks.value(5));
        insertBlock.addBindValue(sourceBlocks.value(6));

        if (!executeQuery(insertBlock, QStringLiteral("Не удалось скопировать блок установки"))) {
            rollbackTransaction(db);
            return;
        }
    }

    if (!commitTransaction(QStringLiteral("Не удалось скопировать установку"), db)) {
        return;
    }

    refreshUnits(newUnitId);
}

void Calculations_Energy_window::moveSelectedUnit(int direction)
{
    if (direction == 0 || showDeletedUnits) {
        return;
    }

    const qint64 unitId = selectedUnitId();
    const int currentIndexValue = selectedUnitIndexValue();
    if (unitId < 0 || currentIndexValue < 0) {
        return;
    }

    if (!ensureDatabaseReady(QStringLiteral("Не удалось переместить установку"))) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    QSqlQuery neighborQuery(db);

    if (direction < 0) {
        neighborQuery.prepare(QStringLiteral(
            "SELECT ID, [Index] FROM Unit "
            "WHERE [Index] >= 0 AND [Index] < ? "
            "ORDER BY [Index] DESC LIMIT 1;"));
    } else {
        neighborQuery.prepare(QStringLiteral(
            "SELECT ID, [Index] FROM Unit "
            "WHERE [Index] >= 0 AND [Index] > ? "
            "ORDER BY [Index] ASC LIMIT 1;"));
    }
    neighborQuery.addBindValue(currentIndexValue);

    if (!executeQuery(neighborQuery, QStringLiteral("Не удалось найти соседнюю установку"))) {
        return;
    }

    if (!neighborQuery.next()) {
        return;
    }

    const qint64 neighborId = neighborQuery.value(0).toLongLong();
    const int neighborIndexValue = neighborQuery.value(1).toInt();

    if (!swapIndexes(QStringLiteral("Unit"),
                     unitId,
                     currentIndexValue,
                     neighborId,
                     neighborIndexValue,
                     QStringLiteral("Не удалось изменить порядок установок"))) {
        return;
    }

    refreshUnits(unitId);
}

void Calculations_Energy_window::toggleDeletedUnitsMode()
{
    showDeletedUnits = !showDeletedUnits;
    refreshUnits();
}

void Calculations_Energy_window::addBlock()
{
    const qint64 unitId = selectedUnitId();
    if (unitId < 0) {
        return;
    }

    if (!ensureDatabaseReady(QStringLiteral("Не удалось добавить блок"))) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    if (!beginTransaction(QStringLiteral("Не удалось добавить блок"), db)) {
        return;
    }

    int blockIndex = 1;
    if (!nextBlockIndex(db, unitId, &blockIndex)) {
        rollbackTransaction(db);
        return;
    }

    QSqlQuery insert(db);
    insert.prepare(QStringLiteral(
        "INSERT INTO Block "
        "(ID_Unit, iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, [Index]) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);"));
    insert.addBindValue(unitId);
    insert.addBindValue(QStringLiteral("Новый блок"));
    insert.addBindValue(0.1);
    insert.addBindValue(false);
    insert.addBindValue(false);
    insert.addBindValue(QString());
    insert.addBindValue(blockIndex);

    if (!executeQuery(insert, QStringLiteral("Не удалось добавить блок"))) {
        rollbackTransaction(db);
        return;
    }

    if (!commitTransaction(QStringLiteral("Не удалось добавить блок"), db)) {
        return;
    }

    refreshBlocks(insert.lastInsertId().toLongLong());
    showNotImplemented(QStringLiteral("Редактор блока"));
}

void Calculations_Energy_window::deleteSelectedBlock()
{
    const qint64 blockId = selectedBlockId();
    if (blockId < 0) {
        return;
    }

    if (!ensureDatabaseReady(QStringLiteral("Не удалось удалить блок"))) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM Block WHERE ID = ?;"));
    query.addBindValue(blockId);

    if (!executeQuery(query, QStringLiteral("Не удалось удалить блок"))) {
        return;
    }

    refreshBlocks();
}

void Calculations_Energy_window::copySelectedBlock()
{
    const qint64 sourceBlockId = selectedBlockId();
    const qint64 unitId = selectedUnitId();
    if (sourceBlockId < 0 || unitId < 0) {
        return;
    }

    if (!ensureDatabaseReady(QStringLiteral("Не удалось скопировать блок"))) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    if (!beginTransaction(QStringLiteral("Не удалось скопировать блок"), db)) {
        return;
    }

    QSqlQuery source(db);
    source.prepare(QStringLiteral(
        "SELECT iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, iDerevoVer_ID, TRIAL922 "
        "FROM Block WHERE ID = ?;"));
    source.addBindValue(sourceBlockId);

    if (!executeQuery(source, QStringLiteral("Не удалось прочитать исходный блок")) || !source.next()) {
        rollbackTransaction(db);
        return;
    }

    int blockIndex = 1;
    if (!nextBlockIndex(db, unitId, &blockIndex)) {
        rollbackTransaction(db);
        return;
    }

    QSqlQuery insert(db);
    insert.prepare(QStringLiteral(
        "INSERT INTO Block "
        "(ID_Unit, iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, [Index], iDerevoVer_ID, TRIAL922) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"));
    insert.addBindValue(unitId);
    insert.addBindValue(source.value(0).toString() + QStringLiteral(" (копия)"));
    insert.addBindValue(source.value(1));
    insert.addBindValue(source.value(2));
    insert.addBindValue(source.value(3));
    insert.addBindValue(source.value(4));
    insert.addBindValue(blockIndex);
    insert.addBindValue(source.value(5));
    insert.addBindValue(source.value(6));

    if (!executeQuery(insert, QStringLiteral("Не удалось создать копию блока"))) {
        rollbackTransaction(db);
        return;
    }

    if (!commitTransaction(QStringLiteral("Не удалось скопировать блок"), db)) {
        return;
    }

    refreshBlocks(insert.lastInsertId().toLongLong());
}

void Calculations_Energy_window::moveSelectedBlock(int direction)
{
    if (direction == 0 || showDeletedUnits) {
        return;
    }

    const qint64 blockId = selectedBlockId();
    const qint64 unitId = selectedUnitId();
    const int currentIndexValue = selectedBlockIndexValue();
    if (blockId < 0 || unitId < 0 || currentIndexValue < 0) {
        return;
    }

    if (!ensureDatabaseReady(QStringLiteral("Не удалось переместить блок"))) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QString::fromLatin1(kMainConnectionName));
    QSqlQuery neighborQuery(db);

    if (direction < 0) {
        neighborQuery.prepare(QStringLiteral(
            "SELECT ID, [Index] FROM Block "
            "WHERE ID_Unit = ? AND [Index] < ? "
            "ORDER BY [Index] DESC LIMIT 1;"));
    } else {
        neighborQuery.prepare(QStringLiteral(
            "SELECT ID, [Index] FROM Block "
            "WHERE ID_Unit = ? AND [Index] > ? "
            "ORDER BY [Index] ASC LIMIT 1;"));
    }
    neighborQuery.addBindValue(unitId);
    neighborQuery.addBindValue(currentIndexValue);

    if (!executeQuery(neighborQuery, QStringLiteral("Не удалось найти соседний блок"))) {
        return;
    }

    if (!neighborQuery.next()) {
        return;
    }

    const qint64 neighborId = neighborQuery.value(0).toLongLong();
    const int neighborIndexValue = neighborQuery.value(1).toInt();

    if (!swapIndexes(QStringLiteral("Block"),
                     blockId,
                     currentIndexValue,
                     neighborId,
                     neighborIndexValue,
                     QStringLiteral("Не удалось изменить порядок блоков"))) {
        return;
    }

    refreshBlocks(blockId);
}

void Calculations_Energy_window::buildInterface()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setWindowTitle(QStringLiteral("Расчеты энергопотенциалов"));
    resize(1180, 660);

    QLabel *titleLabel = new QLabel(QStringLiteral("Расчеты энергопотенциалов и категорий взрывоопасности"), this);
    QFont titleFont;
    titleFont.setPointSize(15);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QLabel *unitsLabel = new QLabel(QStringLiteral("Установки :"), this);
    unitsLabel->setObjectName(QStringLiteral("sectionLabel"));

    QLabel *blocksLabel = new QLabel(QStringLiteral("Блоки :"), this);
    blocksLabel->setObjectName(QStringLiteral("sectionLabel"));

    createUnitsTable();
    createBlocksTable();

    QPushButton *unitEditButton = createButton(QStringLiteral("Редактор"));
    unitAddButton = createButton(QStringLiteral("Добавить"));
    unitDeleteButton = createButton(QStringLiteral("Удалить"));
    unitCopyButton = createButton(QStringLiteral("Копировать"));
    QPushButton *reportButton = createButton(QStringLiteral("Отчёт"));
    unitDeletedButton = createButton(QStringLiteral("Удаленные"));

    unitUpButton = createButton(QStringLiteral("⌃"), 45);
    unitDownButton = createButton(QStringLiteral("⌄"), 45);

    QPushButton *blockEditButton = createButton(QStringLiteral("Редактор"));
    blockAddButton = createButton(QStringLiteral("Добавить"));
    blockDeleteButton = createButton(QStringLiteral("Удалить"));
    blockCopyButton = createButton(QStringLiteral("Копировать"));

    blockUpButton = createButton(QStringLiteral("⌃"), 45);
    blockDownButton = createButton(QStringLiteral("⌄"), 45);

    closeButton = createButton(QStringLiteral("Закрыть"));
    QPushButton *helpButton = createButton(QStringLiteral("Помощь"));

    QWidget *contentPanel = new QWidget(this);
    contentPanel->setObjectName(QStringLiteral("contentPanel"));

    QVBoxLayout *unitSideButtons = new QVBoxLayout;
    unitSideButtons->setSpacing(4);
    unitSideButtons->addWidget(unitEditButton);
    unitSideButtons->addWidget(unitAddButton);
    unitSideButtons->addWidget(unitDeleteButton);
    unitSideButtons->addWidget(unitCopyButton);
    unitSideButtons->addWidget(reportButton);
    unitSideButtons->addSpacing(16);
    unitSideButtons->addWidget(unitDeletedButton);
    unitSideButtons->addStretch();

    QVBoxLayout *unitMoveButtons = new QVBoxLayout;
    unitMoveButtons->setSpacing(4);
    unitMoveButtons->addSpacing(55);
    unitMoveButtons->addWidget(unitUpButton);
    unitMoveButtons->addWidget(unitDownButton);
    unitMoveButtons->addStretch();

    QHBoxLayout *unitsRowLayout = new QHBoxLayout;
    unitsRowLayout->setSpacing(8);
    unitsRowLayout->addWidget(unitsTable, 1);
    unitsRowLayout->addLayout(unitSideButtons);
    unitsRowLayout->addLayout(unitMoveButtons);

    QHBoxLayout *blockBottomButtons = new QHBoxLayout;
    blockBottomButtons->setSpacing(6);
    blockBottomButtons->addSpacing(40);
    blockBottomButtons->addWidget(blockUpButton);
    blockBottomButtons->addWidget(blockDownButton);
    blockBottomButtons->addStretch();
    blockBottomButtons->addWidget(blockEditButton);
    blockBottomButtons->addWidget(blockAddButton);
    blockBottomButtons->addWidget(blockDeleteButton);
    blockBottomButtons->addWidget(blockCopyButton);
    blockBottomButtons->addStretch();

    QVBoxLayout *panelLayout = new QVBoxLayout(contentPanel);
    panelLayout->setContentsMargins(14, 6, 14, 8);
    panelLayout->setSpacing(4);
    panelLayout->addWidget(unitsLabel);
    panelLayout->addLayout(unitsRowLayout);
    panelLayout->addWidget(blocksLabel);
    panelLayout->addWidget(blocksTable);
    panelLayout->addLayout(blockBottomButtons);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(closeButton);
    bottomLayout->addWidget(helpButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(20, 10, 20, 10);
    mainLayout->setSpacing(8);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(contentPanel);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    connect(unitEditButton, &QPushButton::clicked, this, [this]() {
        showNotImplemented(QStringLiteral("Редактор установки"));
    });
    connect(reportButton, &QPushButton::clicked, this, [this]() {
        showNotImplemented(QStringLiteral("Отчёт по установке"));
    });
    connect(unitAddButton, &QPushButton::clicked, this, &Calculations_Energy_window::addUnit);
    connect(unitDeleteButton, &QPushButton::clicked, this, &Calculations_Energy_window::deleteSelectedUnit);
    connect(unitCopyButton, &QPushButton::clicked, this, &Calculations_Energy_window::copySelectedUnit);
    connect(unitDeletedButton, &QPushButton::clicked, this, &Calculations_Energy_window::toggleDeletedUnitsMode);
    connect(unitUpButton, &QPushButton::clicked, this, [this]() { moveSelectedUnit(-1); });
    connect(unitDownButton, &QPushButton::clicked, this, [this]() { moveSelectedUnit(1); });

    connect(blockEditButton, &QPushButton::clicked, this, [this]() {
        showNotImplemented(QStringLiteral("Редактор блока"));
    });
    connect(blockAddButton, &QPushButton::clicked, this, &Calculations_Energy_window::addBlock);
    connect(blockDeleteButton, &QPushButton::clicked, this, &Calculations_Energy_window::deleteSelectedBlock);
    connect(blockCopyButton, &QPushButton::clicked, this, &Calculations_Energy_window::copySelectedBlock);
    connect(blockUpButton, &QPushButton::clicked, this, [this]() { moveSelectedBlock(-1); });
    connect(blockDownButton, &QPushButton::clicked, this, [this]() { moveSelectedBlock(1); });

    connect(helpButton, &QPushButton::clicked, this, [this]() {
        showNotImplemented(QStringLiteral("Контекстная помощь (getHelp(2100))"));
    });

    connect(closeButton, &QPushButton::clicked, this, &Calculations_Energy_window::close);

    setStyleSheet(
        "QMainWindow, QWidget {"
        "    background-color: white;"
        "    color: black;"
        "    font-family: Arial;"
        "    font-size: 13px;"
        "}"

        "QWidget#contentPanel {"
        "    background-color: #eeeeee;"
        "}"

        "QLabel#sectionLabel {"
        "    font-weight: bold;"
        "    background-color: #eeeeee;"
        "}"

        "QTableView {"
        "    background-color: white;"
        "    border: 1px solid #c8c8c8;"
        "    gridline-color: #d0d0d0;"
        "    selection-background-color: black;"
        "    selection-color: white;"
        "}"

        "QTableView::item:selected {"
        "    background-color: black;"
        "    color: white;"
        "}"

        "QTableView::item:selected:!active {"
        "    background-color: black;"
        "    color: white;"
        "}"

        "QHeaderView::section {"
        "    background-color: #eeeeee;"
        "    color: black;"
        "    border: 1px solid #c8c8c8;"
        "    padding: 2px;"
        "    font-weight: bold;"
        "}"

        "QPushButton {"
        "    background-color: #f4f4f4;"
        "    border: 1px solid #a8a8a8;"
        "    padding: 4px 10px;"
        "    color: black;"
        "}"

        "QPushButton:hover {"
        "    background-color: #e7e7e7;"
        "}"

        "QPushButton:pressed {"
        "    background-color: #d7d7d7;"
        "}");
}
