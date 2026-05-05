#include "calculations_of_energy_potentials_window.h"

#include <QAbstractItemView>
#include <QFont>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QVariant>
#include <QVBoxLayout>
#include <QWidget>

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
    unitsModel = new QStandardItemModel(0, 2, unitsTable);

    unitsModel->setHeaderData(0, Qt::Horizontal, "Название установки");
    unitsModel->setHeaderData(1, Qt::Horizontal, "Комментарий");

    unitsTable->setModel(unitsModel);

    unitsTable->verticalHeader()->setVisible(false);
    unitsTable->horizontalHeader()->setStretchLastSection(true);
    unitsTable->horizontalHeader()->setFixedHeight(26);

    unitsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    unitsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    unitsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    unitsTable->setFocusPolicy(Qt::StrongFocus);

    unitsTable->setColumnWidth(0, 520);
    unitsTable->setColumnWidth(1, 420);
    unitsTable->setFixedHeight(190);

    connect(unitsTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [this](const QModelIndex &, const QModelIndex &) {
                refreshBlocks();
                updateActionStates();
            });
}

void Calculations_Energy_window::createBlocksTable()
{
    blocksTable = new QTableView(this);
    blocksModel = new QStandardItemModel(0, 9, blocksTable);

    blocksModel->setHeaderData(0, Qt::Horizontal, "Название блока");
    blocksModel->setHeaderData(1, Qt::Horizontal, "Коэф.\nуч. во\nвзрыве");
    blocksModel->setHeaderData(2, Qt::Horizontal, "Токсичные,\nвысокотокс\nичные в-ва");
    blocksModel->setHeaderData(3, Qt::Horizontal, "Кате-\nгория:");
    blocksModel->setHeaderData(4, Qt::Horizontal, "Полный\nпотен-циал");
    blocksModel->setHeaderData(5, Qt::Horizontal, "Приведен-\nная масса:");
    blocksModel->setHeaderData(6, Qt::Horizontal, "Относитель\nный\nпотенциал:");
    blocksModel->setHeaderData(7, Qt::Horizontal, "Радиус\nRo:");
    blocksModel->setHeaderData(8, Qt::Horizontal, "Оценка\nзон\nпораж.");

    blocksTable->setModel(blocksModel);

    blocksTable->verticalHeader()->setVisible(false);
    blocksTable->horizontalHeader()->setFixedHeight(78);
    blocksTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    blocksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    blocksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    blocksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    blocksTable->setFocusPolicy(Qt::StrongFocus);

    blocksTable->setColumnWidth(0, 360);
    blocksTable->setColumnWidth(1, 70);
    blocksTable->setColumnWidth(2, 95);
    blocksTable->setColumnWidth(3, 70);
    blocksTable->setColumnWidth(4, 100);
    blocksTable->setColumnWidth(5, 100);
    blocksTable->setColumnWidth(6, 115);
    blocksTable->setColumnWidth(7, 75);
    blocksTable->setColumnWidth(8, 80);

    blocksTable->setFixedHeight(200);

    connect(blocksTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [this](const QModelIndex &, const QModelIndex &) {
                updateActionStates();
            });
}

bool Calculations_Energy_window::executeQuery(QSqlQuery &query, const QString &actionName)
{
    if (query.exec()) {
        return true;
    }

    QMessageBox::critical(this,
                          "Ошибка базы данных",
                          QStringLiteral("%1\n\n%2")
                              .arg(actionName, query.lastError().text()));
    return false;
}

qint64 Calculations_Energy_window::selectedUnitId() const
{
    if (!unitsTable || !unitsModel) {
        return -1;
    }

    const QModelIndex index = unitsTable->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    const QStandardItem *item = unitsModel->item(index.row(), 0);
    return item ? item->data(IdRole).toLongLong() : -1;
}

qint64 Calculations_Energy_window::selectedBlockId() const
{
    if (!blocksTable || !blocksModel) {
        return -1;
    }

    const QModelIndex index = blocksTable->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    const QStandardItem *item = blocksModel->item(index.row(), 0);
    return item ? item->data(IdRole).toLongLong() : -1;
}

int Calculations_Energy_window::selectedUnitIndexValue() const
{
    if (!unitsTable || !unitsModel) {
        return -1;
    }

    const QModelIndex index = unitsTable->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    const QStandardItem *item = unitsModel->item(index.row(), 0);
    return item ? item->data(IndexRole).toInt() : -1;
}

int Calculations_Energy_window::selectedBlockIndexValue() const
{
    if (!blocksTable || !blocksModel) {
        return -1;
    }

    const QModelIndex index = blocksTable->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    const QStandardItem *item = blocksModel->item(index.row(), 0);
    return item ? item->data(IndexRole).toInt() : -1;
}

int Calculations_Energy_window::nextUnitIndex() const
{
    QSqlQuery query;
    query.prepare("SELECT COALESCE(MAX([Index]), 0) + 1 FROM Unit;");
    if (!query.exec() || !query.next()) {
        return 1;
    }
    return query.value(0).toInt();
}

int Calculations_Energy_window::nextBlockIndex(qint64 unitId) const
{
    QSqlQuery query;
    query.prepare("SELECT COALESCE(MAX([Index]), 0) + 1 FROM Block WHERE ID_Unit = ?;");
    query.addBindValue(unitId);
    if (!query.exec() || !query.next()) {
        return 1;
    }
    return query.value(0).toInt();
}

void Calculations_Energy_window::refreshUnits(qint64 preferredUnitId)
{
    qint64 unitIdToSelect = preferredUnitId >= 0 ? preferredUnitId : selectedUnitId();

    unitsModel->removeRows(0, unitsModel->rowCount());

    QSqlQuery query;
    if (showDeletedUnits) {
        query.prepare("SELECT ID, iUnitName, iComment, [Index] "
                      "FROM Unit WHERE [Index] < 0 "
                      "ORDER BY DeleteDate DESC, ID DESC;");
    } else {
        query.prepare("SELECT ID, iUnitName, iComment, [Index] "
                      "FROM Unit WHERE [Index] >= 0 "
                      "ORDER BY [Index];");
    }

    if (!executeQuery(query, "Не удалось загрузить список установок")) {
        refreshBlocks();
        updateActionStates();
        return;
    }

    int rowToSelect = -1;
    int row = 0;
    while (query.next()) {
        const qint64 id = query.value(0).toLongLong();
        const QString name = query.value(1).toString();
        const QString comment = query.value(2).toString();
        const int indexValue = query.value(3).toInt();

        QStandardItem *nameItem = new QStandardItem(name);
        QStandardItem *commentItem = new QStandardItem(comment);
        nameItem->setData(id, IdRole);
        nameItem->setData(indexValue, IndexRole);

        unitsModel->appendRow({nameItem, commentItem});

        if (id == unitIdToSelect) {
            rowToSelect = row;
        }
        ++row;
    }

    if (rowToSelect < 0 && unitsModel->rowCount() > 0) {
        rowToSelect = 0;
    }

    if (rowToSelect >= 0) {
        unitsTable->setCurrentIndex(unitsModel->index(rowToSelect, 0));
    } else {
        refreshBlocks();
    }

    if (unitDeletedButton) {
        unitDeletedButton->setText(showDeletedUnits ? "Активные" : "Удаленные");
    }

    updateActionStates();
}

void Calculations_Energy_window::refreshBlocks(qint64 preferredBlockId)
{
    qint64 blockIdToSelect = preferredBlockId >= 0 ? preferredBlockId : selectedBlockId();

    blocksModel->removeRows(0, blocksModel->rowCount());

    const qint64 unitId = selectedUnitId();
    if (unitId < 0) {
        updateActionStates();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID, iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, [Index] "
                  "FROM Block WHERE ID_Unit = ? "
                  "ORDER BY [Index];");
    query.addBindValue(unitId);

    if (!executeQuery(query, "Не удалось загрузить список блоков")) {
        updateActionStates();
        return;
    }

    int rowToSelect = -1;
    int row = 0;
    while (query.next()) {
        const qint64 id = query.value(0).toLongLong();
        const QString blockName = query.value(1).toString();
        const QString kEruption = query.value(2).isNull() ? "-" : query.value(2).toString();
        const bool classOneTwo = query.value(3).toBool();
        const bool printRadius = query.value(4).toBool();
        const int indexValue = query.value(5).toInt();

        QStandardItem *nameItem = new QStandardItem(blockName);
        QStandardItem *kItem = new QStandardItem(kEruption);
        QStandardItem *classItem = new QStandardItem(classOneTwo ? "Да" : "Нет");
        QStandardItem *categoryItem = new QStandardItem("-");
        QStandardItem *fullEnergyItem = new QStandardItem("-");
        QStandardItem *massItem = new QStandardItem("-");
        QStandardItem *relativeItem = new QStandardItem("-");
        QStandardItem *radiusItem = new QStandardItem("-");
        QStandardItem *printItem = new QStandardItem(printRadius ? "Да" : "Нет");

        nameItem->setData(id, IdRole);
        nameItem->setData(indexValue, IndexRole);

        blocksModel->appendRow({nameItem,
                                kItem,
                                classItem,
                                categoryItem,
                                fullEnergyItem,
                                massItem,
                                relativeItem,
                                radiusItem,
                                printItem});

        if (id == blockIdToSelect) {
            rowToSelect = row;
        }
        ++row;
    }

    if (rowToSelect < 0 && blocksModel->rowCount() > 0) {
        rowToSelect = 0;
    }

    if (rowToSelect >= 0) {
        blocksTable->setCurrentIndex(blocksModel->index(rowToSelect, 0));
    }

    updateActionStates();
}

void Calculations_Energy_window::updateActionStates()
{
    const bool hasUnit = selectedUnitId() >= 0;
    const bool hasBlock = selectedBlockId() >= 0;
    const bool allowEdits = !showDeletedUnits;

    if (unitAddButton) {
        unitAddButton->setEnabled(allowEdits);
    }
    if (unitDeleteButton) {
        unitDeleteButton->setEnabled(allowEdits && hasUnit);
    }
    if (unitCopyButton) {
        unitCopyButton->setEnabled(allowEdits && hasUnit);
    }
    if (unitUpButton) {
        unitUpButton->setEnabled(allowEdits && hasUnit);
    }
    if (unitDownButton) {
        unitDownButton->setEnabled(allowEdits && hasUnit);
    }

    if (blockAddButton) {
        blockAddButton->setEnabled(allowEdits && hasUnit);
    }
    if (blockDeleteButton) {
        blockDeleteButton->setEnabled(allowEdits && hasBlock);
    }
    if (blockCopyButton) {
        blockCopyButton->setEnabled(allowEdits && hasBlock);
    }
    if (blockUpButton) {
        blockUpButton->setEnabled(allowEdits && hasBlock);
    }
    if (blockDownButton) {
        blockDownButton->setEnabled(allowEdits && hasBlock);
    }
}

void Calculations_Energy_window::showNotImplemented(const QString &actionName)
{
    QMessageBox::information(this,
                             "Функция в разработке",
                             QStringLiteral("%1 будет реализовано на следующем этапе.").arg(actionName));
}

void Calculations_Energy_window::addUnit()
{
    QSqlQuery insert;
    insert.prepare("INSERT INTO Unit (iUnitName, iComment, [Index], DeleteDate) "
                   "VALUES (?, ?, ?, NULL);");
    insert.addBindValue(QStringLiteral("Новая установка"));
    insert.addBindValue(QString());
    insert.addBindValue(nextUnitIndex());

    if (!executeQuery(insert, "Не удалось добавить установку")) {
        return;
    }

    const qint64 newUnitId = insert.lastInsertId().toLongLong();
    refreshUnits(newUnitId);
    showNotImplemented("Редактор установки");
}

void Calculations_Energy_window::deleteSelectedUnit()
{
    const qint64 unitId = selectedUnitId();
    if (unitId < 0) {
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE Unit SET [Index] = -1, DeleteDate = CURRENT_TIMESTAMP WHERE ID = ?;");
    query.addBindValue(unitId);
    if (!executeQuery(query, "Не удалось удалить установку")) {
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

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось начать транзакцию копирования.");
        return;
    }

    QSqlQuery sourceUnit;
    sourceUnit.prepare("SELECT iUnitName, iComment, iDocuments, iDiscription, TRIAL922 "
                       "FROM Unit WHERE ID = ?;");
    sourceUnit.addBindValue(sourceUnitId);
    if (!executeQuery(sourceUnit, "Не удалось прочитать исходную установку") || !sourceUnit.next()) {
        db.rollback();
        return;
    }

    QSqlQuery insertUnit;
    insertUnit.prepare("INSERT INTO Unit (iUnitName, iComment, iDocuments, iDiscription, [Index], DeleteDate, TRIAL922) "
                       "VALUES (?, ?, ?, ?, ?, NULL, ?);");
    insertUnit.addBindValue(sourceUnit.value(0).toString() + QStringLiteral(" (копия)"));
    insertUnit.addBindValue(sourceUnit.value(1));
    insertUnit.addBindValue(sourceUnit.value(2));
    insertUnit.addBindValue(sourceUnit.value(3));
    insertUnit.addBindValue(nextUnitIndex());
    insertUnit.addBindValue(sourceUnit.value(4));

    if (!executeQuery(insertUnit, "Не удалось создать копию установки")) {
        db.rollback();
        return;
    }

    const qint64 newUnitId = insertUnit.lastInsertId().toLongLong();

    QSqlQuery sourceBlocks;
    sourceBlocks.prepare("SELECT iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, iDerevoVer_ID, TRIAL922 "
                         "FROM Block WHERE ID_Unit = ? "
                         "ORDER BY [Index];");
    sourceBlocks.addBindValue(sourceUnitId);
    if (!executeQuery(sourceBlocks, "Не удалось прочитать блоки исходной установки")) {
        db.rollback();
        return;
    }

    int indexValue = nextBlockIndex(newUnitId);
    while (sourceBlocks.next()) {
        QSqlQuery insertBlock;
        insertBlock.prepare("INSERT INTO Block "
                            "(ID_Unit, iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, [Index], iDerevoVer_ID, TRIAL922) "
                            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);");
        insertBlock.addBindValue(newUnitId);
        insertBlock.addBindValue(sourceBlocks.value(0).toString() + QStringLiteral(" (копия)"));
        insertBlock.addBindValue(sourceBlocks.value(1));
        insertBlock.addBindValue(sourceBlocks.value(2));
        insertBlock.addBindValue(sourceBlocks.value(3));
        insertBlock.addBindValue(sourceBlocks.value(4));
        insertBlock.addBindValue(indexValue++);
        insertBlock.addBindValue(sourceBlocks.value(5));
        insertBlock.addBindValue(sourceBlocks.value(6));

        if (!executeQuery(insertBlock, "Не удалось скопировать блок установки")) {
            db.rollback();
            return;
        }
    }

    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось завершить транзакцию копирования.");
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

    QSqlQuery neighborQuery;
    if (direction < 0) {
        neighborQuery.prepare("SELECT ID, [Index] FROM Unit "
                              "WHERE [Index] >= 0 AND [Index] < ? "
                              "ORDER BY [Index] DESC LIMIT 1;");
    } else {
        neighborQuery.prepare("SELECT ID, [Index] FROM Unit "
                              "WHERE [Index] >= 0 AND [Index] > ? "
                              "ORDER BY [Index] ASC LIMIT 1;");
    }
    neighborQuery.addBindValue(currentIndexValue);

    if (!executeQuery(neighborQuery, "Не удалось найти соседнюю установку")) {
        return;
    }

    if (!neighborQuery.next()) {
        return;
    }

    const qint64 neighborId = neighborQuery.value(0).toLongLong();
    const int neighborIndexValue = neighborQuery.value(1).toInt();

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось начать транзакцию перестановки.");
        return;
    }

    QSqlQuery updateCurrent;
    updateCurrent.prepare("UPDATE Unit SET [Index] = ? WHERE ID = ?;");
    updateCurrent.addBindValue(neighborIndexValue);
    updateCurrent.addBindValue(unitId);
    if (!executeQuery(updateCurrent, "Не удалось обновить индекс текущей установки")) {
        db.rollback();
        return;
    }

    QSqlQuery updateNeighbor;
    updateNeighbor.prepare("UPDATE Unit SET [Index] = ? WHERE ID = ?;");
    updateNeighbor.addBindValue(currentIndexValue);
    updateNeighbor.addBindValue(neighborId);
    if (!executeQuery(updateNeighbor, "Не удалось обновить индекс соседней установки")) {
        db.rollback();
        return;
    }

    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось завершить перестановку установок.");
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

    QSqlQuery insert;
    insert.prepare("INSERT INTO Block "
                   "(ID_Unit, iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, [Index]) "
                   "VALUES (?, ?, ?, ?, ?, ?, ?);");
    insert.addBindValue(unitId);
    insert.addBindValue(QStringLiteral("Новый блок"));
    insert.addBindValue(0.1);
    insert.addBindValue(false);
    insert.addBindValue(false);
    insert.addBindValue(QString());
    insert.addBindValue(nextBlockIndex(unitId));

    if (!executeQuery(insert, "Не удалось добавить блок")) {
        return;
    }

    refreshBlocks(insert.lastInsertId().toLongLong());
    showNotImplemented("Редактор блока");
}

void Calculations_Energy_window::deleteSelectedBlock()
{
    const qint64 blockId = selectedBlockId();
    if (blockId < 0) {
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM Block WHERE ID = ?;");
    query.addBindValue(blockId);
    if (!executeQuery(query, "Не удалось удалить блок")) {
        return;
    }

    refreshBlocks();
}

void Calculations_Energy_window::copySelectedBlock()
{
    const qint64 blockId = selectedBlockId();
    const qint64 unitId = selectedUnitId();
    if (blockId < 0 || unitId < 0) {
        return;
    }

    QSqlQuery source;
    source.prepare("SELECT iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, iDerevoVer_ID, TRIAL922 "
                   "FROM Block WHERE ID = ?;");
    source.addBindValue(blockId);
    if (!executeQuery(source, "Не удалось прочитать исходный блок") || !source.next()) {
        return;
    }

    QSqlQuery insert;
    insert.prepare("INSERT INTO Block "
                   "(ID_Unit, iBlock_Name, iK_eruption, [iClass(I_II)_Yes_No], iPrintR, iDiscription, [Index], iDerevoVer_ID, TRIAL922) "
                   "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);");
    insert.addBindValue(unitId);
    insert.addBindValue(source.value(0).toString() + QStringLiteral(" (копия)"));
    insert.addBindValue(source.value(1));
    insert.addBindValue(source.value(2));
    insert.addBindValue(source.value(3));
    insert.addBindValue(source.value(4));
    insert.addBindValue(nextBlockIndex(unitId));
    insert.addBindValue(source.value(5));
    insert.addBindValue(source.value(6));

    if (!executeQuery(insert, "Не удалось скопировать блок")) {
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

    QSqlQuery neighborQuery;
    if (direction < 0) {
        neighborQuery.prepare("SELECT ID, [Index] FROM Block "
                              "WHERE ID_Unit = ? AND [Index] < ? "
                              "ORDER BY [Index] DESC LIMIT 1;");
    } else {
        neighborQuery.prepare("SELECT ID, [Index] FROM Block "
                              "WHERE ID_Unit = ? AND [Index] > ? "
                              "ORDER BY [Index] ASC LIMIT 1;");
    }
    neighborQuery.addBindValue(unitId);
    neighborQuery.addBindValue(currentIndexValue);

    if (!executeQuery(neighborQuery, "Не удалось найти соседний блок")) {
        return;
    }

    if (!neighborQuery.next()) {
        return;
    }

    const qint64 neighborId = neighborQuery.value(0).toLongLong();
    const int neighborIndexValue = neighborQuery.value(1).toInt();

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось начать транзакцию перестановки.");
        return;
    }

    QSqlQuery updateCurrent;
    updateCurrent.prepare("UPDATE Block SET [Index] = ? WHERE ID = ?;");
    updateCurrent.addBindValue(neighborIndexValue);
    updateCurrent.addBindValue(blockId);
    if (!executeQuery(updateCurrent, "Не удалось обновить индекс текущего блока")) {
        db.rollback();
        return;
    }

    QSqlQuery updateNeighbor;
    updateNeighbor.prepare("UPDATE Block SET [Index] = ? WHERE ID = ?;");
    updateNeighbor.addBindValue(currentIndexValue);
    updateNeighbor.addBindValue(neighborId);
    if (!executeQuery(updateNeighbor, "Не удалось обновить индекс соседнего блока")) {
        db.rollback();
        return;
    }

    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось завершить перестановку блоков.");
        return;
    }

    refreshBlocks(blockId);
}

void Calculations_Energy_window::buildInterface()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setWindowTitle("Расчеты энергопотенциалов");
    resize(1180, 660);

    QLabel *titleLabel = new QLabel("Расчеты энергопотенциалов и категорий взрывоопасности", this);
    QFont titleFont;
    titleFont.setPointSize(15);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QLabel *unitsLabel = new QLabel("Установки :", this);
    unitsLabel->setObjectName("sectionLabel");

    QLabel *blocksLabel = new QLabel("Блоки :", this);
    blocksLabel->setObjectName("sectionLabel");

    createUnitsTable();
    createBlocksTable();

    QPushButton *unitEditButton = createButton("Редактор");
    unitAddButton = createButton("Добавить");
    unitDeleteButton = createButton("Удалить");
    unitCopyButton = createButton("Копировать");
    QPushButton *reportButton = createButton("Отчёт");
    unitDeletedButton = createButton("Удаленные");

    unitUpButton = createButton("⌃", 45);
    unitDownButton = createButton("⌄", 45);

    QPushButton *blockEditButton = createButton("Редактор");
    blockAddButton = createButton("Добавить");
    blockDeleteButton = createButton("Удалить");
    blockCopyButton = createButton("Копировать");

    blockUpButton = createButton("⌃", 45);
    blockDownButton = createButton("⌄", 45);

    closeButton = createButton("Закрыть");
    QPushButton *helpButton = createButton("Помощь");

    QWidget *contentPanel = new QWidget(this);
    contentPanel->setObjectName("contentPanel");

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
        showNotImplemented("Редактор установки");
    });
    connect(reportButton, &QPushButton::clicked, this, [this]() {
        showNotImplemented("Отчёт по установке");
    });
    connect(unitAddButton, &QPushButton::clicked, this, &Calculations_Energy_window::addUnit);
    connect(unitDeleteButton, &QPushButton::clicked, this, &Calculations_Energy_window::deleteSelectedUnit);
    connect(unitCopyButton, &QPushButton::clicked, this, &Calculations_Energy_window::copySelectedUnit);
    connect(unitDeletedButton, &QPushButton::clicked, this, &Calculations_Energy_window::toggleDeletedUnitsMode);
    connect(unitUpButton, &QPushButton::clicked, this, [this]() {
        moveSelectedUnit(-1);
    });
    connect(unitDownButton, &QPushButton::clicked, this, [this]() {
        moveSelectedUnit(1);
    });

    connect(blockEditButton, &QPushButton::clicked, this, [this]() {
        showNotImplemented("Редактор блока");
    });
    connect(blockAddButton, &QPushButton::clicked, this, &Calculations_Energy_window::addBlock);
    connect(blockDeleteButton, &QPushButton::clicked, this, &Calculations_Energy_window::deleteSelectedBlock);
    connect(blockCopyButton, &QPushButton::clicked, this, &Calculations_Energy_window::copySelectedBlock);
    connect(blockUpButton, &QPushButton::clicked, this, [this]() {
        moveSelectedBlock(-1);
    });
    connect(blockDownButton, &QPushButton::clicked, this, [this]() {
        moveSelectedBlock(1);
    });

    connect(helpButton, &QPushButton::clicked, this, [this]() {
        showNotImplemented("Контекстная помощь (getHelp(2100))");
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
