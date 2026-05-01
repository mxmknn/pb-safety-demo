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
#include <QModelIndex>
#include <QFont>

Calculations_Energy_window::Calculations_Energy_window(QWidget *parent)
    : QMainWindow(parent)
{
    buildInterface();
    setAttribute(Qt::WA_DeleteOnClose);
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
    unitsModel = new QStandardItemModel(3, 2, unitsTable);

    unitsModel->setHeaderData(0, Qt::Horizontal, "Название установки");
    unitsModel->setHeaderData(1, Qt::Horizontal, "Комментарий");

    unitsModel->setItem(0, 0, new QStandardItem("Аппарат с газовой фазой"));
    unitsModel->setItem(0, 1, new QStandardItem("Пример расчета E1'"));

    unitsModel->setItem(1, 0, new QStandardItem("Демонстрационная установка"));
    unitsModel->setItem(1, 1, new QStandardItem("Расчет всех энергий"));

    unitsModel->setItem(2, 0, new QStandardItem("Стадия компрессии (блок для расчета сценария риска)"));
    unitsModel->setItem(2, 1, new QStandardItem("Пример расчета E1'"));

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
}

void Calculations_Energy_window::createBlocksTable()
{
    blocksTable = new QTableView(this);
    blocksModel = new QStandardItemModel(1, 9, blocksTable);

    blocksModel->setHeaderData(0, Qt::Horizontal, "Название блока");
    blocksModel->setHeaderData(1, Qt::Horizontal, "Коэф.\nуч. во\nвзрыве");
    blocksModel->setHeaderData(2, Qt::Horizontal, "Токсичные,\nвысокотокс\nичные в-ва");
    blocksModel->setHeaderData(3, Qt::Horizontal, "Кате-\nгория:");
    blocksModel->setHeaderData(4, Qt::Horizontal, "Полный\nпотен-циал");
    blocksModel->setHeaderData(5, Qt::Horizontal, "Приведен-\nная масса:");
    blocksModel->setHeaderData(6, Qt::Horizontal, "Относитель\nный\nпотенциал:");
    blocksModel->setHeaderData(7, Qt::Horizontal, "Радиус\nRo:");
    blocksModel->setHeaderData(8, Qt::Horizontal, "Оценка\nзон\nпораж.");

    blocksModel->setItem(0, 0, new QStandardItem("Аппарат с газовой фазой"));
    blocksModel->setItem(0, 1, new QStandardItem("0,1"));
    blocksModel->setItem(0, 2, new QStandardItem("☐"));
    blocksModel->setItem(0, 3, new QStandardItem("2"));
    blocksModel->setItem(0, 4, new QStandardItem("1,28E+08"));
    blocksModel->setItem(0, 5, new QStandardItem("2,78E+03"));
    blocksModel->setItem(0, 6, new QStandardItem("30,49"));
    blocksModel->setItem(0, 7, new QStandardItem("7,741"));
    blocksModel->setItem(0, 8, new QStandardItem("☑"));

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
}

void Calculations_Energy_window::addUnitRow()
{
    int row = unitsModel->rowCount();

    unitsModel->insertRow(row);
    unitsModel->setItem(row, 0, new QStandardItem("Новая установка"));
    unitsModel->setItem(row, 1, new QStandardItem("Комментарий"));

    unitsTable->setCurrentIndex(unitsModel->index(row, 0));
}

void Calculations_Energy_window::deleteSelectedUnitRow()
{
    QModelIndex index = unitsTable->currentIndex();
    if (!index.isValid()) return;

    int row = index.row();
    unitsModel->removeRow(row);

    if (unitsModel->rowCount() > 0) {
        int nextRow = row;
        if (nextRow >= unitsModel->rowCount()) {
            nextRow = unitsModel->rowCount() - 1;
        }

        unitsTable->setCurrentIndex(unitsModel->index(nextRow, 0));
    }
}

void Calculations_Energy_window::copySelectedUnitRow()
{
    QModelIndex index = unitsTable->currentIndex();
    if (!index.isValid()) return;

    int sourceRow = index.row();
    int newRow = unitsModel->rowCount();

    unitsModel->insertRow(newRow);

    for (int col = 0; col < unitsModel->columnCount(); ++col) {
        QStandardItem *sourceItem = unitsModel->item(sourceRow, col);
        unitsModel->setItem(newRow, col, sourceItem ? sourceItem->clone() : new QStandardItem(""));
    }

    unitsTable->setCurrentIndex(unitsModel->index(newRow, 0));
}

void Calculations_Energy_window::addBlockRow()
{
    int row = blocksModel->rowCount();

    blocksModel->insertRow(row);

    blocksModel->setItem(row, 0, new QStandardItem("Новый блок"));
    blocksModel->setItem(row, 1, new QStandardItem("0,1"));
    blocksModel->setItem(row, 2, new QStandardItem("☐"));
    blocksModel->setItem(row, 3, new QStandardItem("2"));
    blocksModel->setItem(row, 4, new QStandardItem("0"));
    blocksModel->setItem(row, 5, new QStandardItem("0"));
    blocksModel->setItem(row, 6, new QStandardItem("0"));
    blocksModel->setItem(row, 7, new QStandardItem("0"));
    blocksModel->setItem(row, 8, new QStandardItem("☐"));

    blocksTable->setCurrentIndex(blocksModel->index(row, 0));
}

void Calculations_Energy_window::deleteSelectedBlockRow()
{
    QModelIndex index = blocksTable->currentIndex();
    if (!index.isValid()) return;

    int row = index.row();
    blocksModel->removeRow(row);

    if (blocksModel->rowCount() > 0) {
        int nextRow = row;
        if (nextRow >= blocksModel->rowCount()) {
            nextRow = blocksModel->rowCount() - 1;
        }

        blocksTable->setCurrentIndex(blocksModel->index(nextRow, 0));
    }
}

void Calculations_Energy_window::copySelectedBlockRow()
{
    QModelIndex index = blocksTable->currentIndex();
    if (!index.isValid()) return;

    int sourceRow = index.row();
    int newRow = blocksModel->rowCount();

    blocksModel->insertRow(newRow);

    for (int col = 0; col < blocksModel->columnCount(); ++col) {
        QStandardItem *sourceItem = blocksModel->item(sourceRow, col);
        blocksModel->setItem(newRow, col, sourceItem ? sourceItem->clone() : new QStandardItem(""));
    }

    blocksTable->setCurrentIndex(blocksModel->index(newRow, 0));
}

void Calculations_Energy_window::selectRowByDirection(QTableView *table,
                                                      QStandardItemModel *model,
                                                      int direction)
{
    if (!table || !model) return;

    QModelIndex index = table->currentIndex();

    int currentRow = index.isValid() ? index.row() : 0;
    int newRow = currentRow + direction;

    if (newRow < 0 || newRow >= model->rowCount()) return;

    table->setCurrentIndex(model->index(newRow, 0));
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
    QPushButton *unitAddButton = createButton("Добавить");
    QPushButton *unitDeleteButton = createButton("Удалить");
    QPushButton *unitCopyButton = createButton("Копировать");
    QPushButton *reportButton = createButton("Отчёт");
    QPushButton *deletedButton = createButton("Удаленные");

    QPushButton *unitUpButton = createButton("⌃", 45);
    QPushButton *unitDownButton = createButton("⌄", 45);

    QPushButton *blockEditButton = createButton("Редактор");
    QPushButton *blockAddButton = createButton("Добавить");
    QPushButton *blockDeleteButton = createButton("Удалить");
    QPushButton *blockCopyButton = createButton("Копировать");

    QPushButton *blockUpButton = createButton("⌃", 45);
    QPushButton *blockDownButton = createButton("⌄", 45);

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
    unitSideButtons->addWidget(deletedButton);
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

    connect(unitAddButton, &QPushButton::clicked, this, &Calculations_Energy_window::addUnitRow);
    connect(unitDeleteButton, &QPushButton::clicked, this, &Calculations_Energy_window::deleteSelectedUnitRow);
    connect(unitCopyButton, &QPushButton::clicked, this, &Calculations_Energy_window::copySelectedUnitRow);

    connect(unitUpButton, &QPushButton::clicked, this, [this]() {
        selectRowByDirection(unitsTable, unitsModel, -1);
    });

    connect(unitDownButton, &QPushButton::clicked, this, [this]() {
        selectRowByDirection(unitsTable, unitsModel, 1);
    });

    connect(blockAddButton, &QPushButton::clicked, this, &Calculations_Energy_window::addBlockRow);
    connect(blockDeleteButton, &QPushButton::clicked, this, &Calculations_Energy_window::deleteSelectedBlockRow);
    connect(blockCopyButton, &QPushButton::clicked, this, &Calculations_Energy_window::copySelectedBlockRow);

    connect(blockUpButton, &QPushButton::clicked, this, [this]() {
        selectRowByDirection(blocksTable, blocksModel, -1);
    });

    connect(blockDownButton, &QPushButton::clicked, this, [this]() {
        selectRowByDirection(blocksTable, blocksModel, 1);
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
        "}"
        );
}