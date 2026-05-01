#include "calculations_of_energy_potentials_window.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QItemSelectionModel>

Calculations_Energy_window::Calculations_Energy_window(QWidget *parent)
    : QMainWindow(parent)
{
    buildInterface();
    setAttribute(Qt::WA_DeleteOnClose);
}

void Calculations_Energy_window::createUnitsTable()
{
    unitsTable = new QTableView(this);
    unitsModel = new QStandardItemModel(3, 2, unitsTable);

    unitsModel->setHeaderData(0, Qt::Horizontal, "Название установки");
    unitsModel->setHeaderData(1, Qt::Horizontal, "Комментарий");

    unitsModel->setItem(0, 0, new QStandardItem("Аппарат с газовой фазой"));
    unitsModel->setItem(0, 1, new QStandardItem("Пример расчета E1"));

    unitsModel->setItem(1, 0, new QStandardItem("Демонстрационная установка"));
    unitsModel->setItem(1, 1, new QStandardItem("Расчет всех энергий"));

    unitsModel->setItem(2, 0, new QStandardItem("Стадия компрессии"));
    unitsModel->setItem(2, 1, new QStandardItem("Пример расчета E1"));

    unitsTable->setModel(unitsModel);

    unitsTable->verticalHeader()->setVisible(false);
    unitsTable->horizontalHeader()->setStretchLastSection(true);

    unitsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    unitsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    unitsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    unitsTable->selectRow(0);
}

void Calculations_Energy_window::createBlocksTable()
{
    blocksTable = new QTableView(this);
    blocksModel = new QStandardItemModel(1, 9, blocksTable);

    blocksModel->setHeaderData(0, Qt::Horizontal, "Название блока");
    blocksModel->setHeaderData(1, Qt::Horizontal, "Коэф.");
    blocksModel->setHeaderData(2, Qt::Horizontal, "Токсичные в-ва");
    blocksModel->setHeaderData(3, Qt::Horizontal, "Категория");
    blocksModel->setHeaderData(4, Qt::Horizontal, "Полный потенциал");
    blocksModel->setHeaderData(5, Qt::Horizontal, "Приведенная масса");
    blocksModel->setHeaderData(6, Qt::Horizontal, "Относительный потенциал");
    blocksModel->setHeaderData(7, Qt::Horizontal, "Радиус Ro");
    blocksModel->setHeaderData(8, Qt::Horizontal, "Оценка зон");

    blocksModel->setItem(0, 0, new QStandardItem("Аппарат с газовой фазой"));
    blocksModel->setItem(0, 1, new QStandardItem("0,1"));
    blocksModel->setItem(0, 2, new QStandardItem("Нет"));
    blocksModel->setItem(0, 3, new QStandardItem("2"));
    blocksModel->setItem(0, 4, new QStandardItem("1,28E+08"));
    blocksModel->setItem(0, 5, new QStandardItem("2,78E+03"));
    blocksModel->setItem(0, 6, new QStandardItem("30,49"));
    blocksModel->setItem(0, 7, new QStandardItem("7,741"));
    blocksModel->setItem(0, 8, new QStandardItem("Да"));

    blocksTable->setModel(blocksModel);

    blocksTable->verticalHeader()->setVisible(false);

    blocksTable->setColumnWidth(0, 260);
    blocksTable->setColumnWidth(1, 80);
    blocksTable->setColumnWidth(2, 120);
    blocksTable->setColumnWidth(3, 90);
    blocksTable->setColumnWidth(4, 140);
    blocksTable->setColumnWidth(5, 140);
    blocksTable->setColumnWidth(6, 160);
    blocksTable->setColumnWidth(7, 90);
    blocksTable->setColumnWidth(8, 100);

    blocksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    blocksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    blocksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    blocksTable->selectRow(0);
}

void Calculations_Energy_window::addUnitRow()
{
    const int row = unitsModel->rowCount();

    unitsModel->insertRow(row);
    unitsModel->setItem(row, 0, new QStandardItem("Новая установка"));
    unitsModel->setItem(row, 1, new QStandardItem("Комментарий"));

    unitsTable->selectRow(row);
}

void Calculations_Energy_window::deleteSelectedUnitRow()
{
    const QModelIndex index = unitsTable->currentIndex();
    if (!index.isValid()) return;

    unitsModel->removeRow(index.row());

    if (unitsModel->rowCount() > 0) {
        const int row = qMin(index.row(), unitsModel->rowCount() - 1);
        unitsTable->selectRow(row);
    }
}

void Calculations_Energy_window::copySelectedUnitRow()
{
    const QModelIndex index = unitsTable->currentIndex();
    if (!index.isValid()) return;

    const int sourceRow = index.row();
    const int newRow = unitsModel->rowCount();

    unitsModel->insertRow(newRow);

    for (int col = 0; col < unitsModel->columnCount(); ++col) {
        QStandardItem *sourceItem = unitsModel->item(sourceRow, col);

        if (sourceItem) {
            unitsModel->setItem(newRow, col, sourceItem->clone());
        } else {
            unitsModel->setItem(newRow, col, new QStandardItem(""));
        }
    }

    unitsTable->selectRow(newRow);
}

void Calculations_Energy_window::addBlockRow()
{
    const int row = blocksModel->rowCount();

    blocksModel->insertRow(row);

    blocksModel->setItem(row, 0, new QStandardItem("Новый блок"));
    blocksModel->setItem(row, 1, new QStandardItem("0,1"));
    blocksModel->setItem(row, 2, new QStandardItem("Нет"));
    blocksModel->setItem(row, 3, new QStandardItem("2"));
    blocksModel->setItem(row, 4, new QStandardItem("0"));
    blocksModel->setItem(row, 5, new QStandardItem("0"));
    blocksModel->setItem(row, 6, new QStandardItem("0"));
    blocksModel->setItem(row, 7, new QStandardItem("0"));
    blocksModel->setItem(row, 8, new QStandardItem("Нет"));

    blocksTable->selectRow(row);
}

void Calculations_Energy_window::deleteSelectedBlockRow()
{
    const QModelIndex index = blocksTable->currentIndex();
    if (!index.isValid()) return;

    blocksModel->removeRow(index.row());

    if (blocksModel->rowCount() > 0) {
        const int row = qMin(index.row(), blocksModel->rowCount() - 1);
        blocksTable->selectRow(row);
    }
}

void Calculations_Energy_window::copySelectedBlockRow()
{
    const QModelIndex index = blocksTable->currentIndex();
    if (!index.isValid()) return;

    const int sourceRow = index.row();
    const int newRow = blocksModel->rowCount();

    blocksModel->insertRow(newRow);

    for (int col = 0; col < blocksModel->columnCount(); ++col) {
        QStandardItem *sourceItem = blocksModel->item(sourceRow, col);

        if (sourceItem) {
            blocksModel->setItem(newRow, col, sourceItem->clone());
        } else {
            blocksModel->setItem(newRow, col, new QStandardItem(""));
        }
    }

    blocksTable->selectRow(newRow);
}

void Calculations_Energy_window::moveSelectedRow(QTableView *table, QStandardItemModel *model, int direction)
{
    if (!table || !model) return;

    const QModelIndex index = table->currentIndex();
    if (!index.isValid()) return;

    const int row = index.row();
    const int targetRow = row + direction;

    if (targetRow < 0 || targetRow >= model->rowCount()) return;

    QList<QStandardItem *> currentItems;
    QList<QStandardItem *> targetItems;

    currentItems.reserve(model->columnCount());
    targetItems.reserve(model->columnCount());

    for (int col = 0; col < model->columnCount(); ++col) {
        QStandardItem *currentItem = model->takeItem(row, col);
        QStandardItem *targetItem = model->takeItem(targetRow, col);

        currentItems.append(currentItem ? currentItem : new QStandardItem(""));
        targetItems.append(targetItem ? targetItem : new QStandardItem(""));
    }

    for (int col = 0; col < model->columnCount(); ++col) {
        model->setItem(row, col, targetItems[col]);
        model->setItem(targetRow, col, currentItems[col]);
    }

    table->selectRow(targetRow);
}

void Calculations_Energy_window::buildInterface()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setWindowTitle("Расчеты энергопотенциалов");
    resize(1150, 700);

    QLabel *unitsLabel = new QLabel("Установки :", this);
    QLabel *blocksLabel = new QLabel("Блоки :", this);

    createUnitsTable();
    createBlocksTable();

    QPushButton *unitAddButton = new QPushButton("Добавить", this);
    QPushButton *unitDeleteButton = new QPushButton("Удалить", this);
    QPushButton *unitCopyButton = new QPushButton("Копировать", this);
    QPushButton *unitUpButton = new QPushButton("Вверх", this);
    QPushButton *unitDownButton = new QPushButton("Вниз", this);

    QPushButton *blockAddButton = new QPushButton("Добавить", this);
    QPushButton *blockDeleteButton = new QPushButton("Удалить", this);
    QPushButton *blockCopyButton = new QPushButton("Копировать", this);
    QPushButton *blockUpButton = new QPushButton("Вверх", this);
    QPushButton *blockDownButton = new QPushButton("Вниз", this);

    closeButton = new QPushButton("Закрыть", this);

    QHBoxLayout *unitButtonsLayout = new QHBoxLayout;
    unitButtonsLayout->addWidget(unitAddButton);
    unitButtonsLayout->addWidget(unitDeleteButton);
    unitButtonsLayout->addWidget(unitCopyButton);
    unitButtonsLayout->addWidget(unitUpButton);
    unitButtonsLayout->addWidget(unitDownButton);
    unitButtonsLayout->addStretch();

    QHBoxLayout *blockButtonsLayout = new QHBoxLayout;
    blockButtonsLayout->addWidget(blockAddButton);
    blockButtonsLayout->addWidget(blockDeleteButton);
    blockButtonsLayout->addWidget(blockCopyButton);
    blockButtonsLayout->addWidget(blockUpButton);
    blockButtonsLayout->addWidget(blockDownButton);
    blockButtonsLayout->addStretch();

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    mainLayout->addWidget(unitsLabel);
    mainLayout->addWidget(unitsTable);
    mainLayout->addLayout(unitButtonsLayout);

    mainLayout->addWidget(blocksLabel);
    mainLayout->addWidget(blocksTable);
    mainLayout->addLayout(blockButtonsLayout);

    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    connect(unitAddButton, &QPushButton::clicked, this, &Calculations_Energy_window::addUnitRow);
    connect(unitDeleteButton, &QPushButton::clicked, this, &Calculations_Energy_window::deleteSelectedUnitRow);
    connect(unitCopyButton, &QPushButton::clicked, this, &Calculations_Energy_window::copySelectedUnitRow);

    connect(unitUpButton, &QPushButton::clicked, this, [this]() {
        moveSelectedRow(unitsTable, unitsModel, -1);
    });

    connect(unitDownButton, &QPushButton::clicked, this, [this]() {
        moveSelectedRow(unitsTable, unitsModel, 1);
    });

    connect(blockAddButton, &QPushButton::clicked, this, &Calculations_Energy_window::addBlockRow);
    connect(blockDeleteButton, &QPushButton::clicked, this, &Calculations_Energy_window::deleteSelectedBlockRow);
    connect(blockCopyButton, &QPushButton::clicked, this, &Calculations_Energy_window::copySelectedBlockRow);

    connect(blockUpButton, &QPushButton::clicked, this, [this]() {
        moveSelectedRow(blocksTable, blocksModel, -1);
    });

    connect(blockDownButton, &QPushButton::clicked, this, [this]() {
        moveSelectedRow(blocksTable, blocksModel, 1);
    });

    connect(closeButton, &QPushButton::clicked, this, &Calculations_Energy_window::close);
}