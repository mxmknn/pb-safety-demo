// #include "calculations_of_energy_potentials_window.h"
// #include <QWidget>
// #include <QTableWidget>
// #include <QTableWidgetItem>
// #include <QPushButton>
// #include <QLabel>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QGroupBox>
// #include <QHeaderView>
// #include <QSplitter>
// #include <QFont>
// #include <QFile>
// #include <QTextStream>
// #include <QCoreApplication>

// static void debugLog(const QString &text)
// {
//     QFile file(QCoreApplication::applicationDirPath() + "/debug_log.txt");

//     if (file.open(QIODevice::Append | QIODevice::Text)) {
//         QTextStream out(&file);
//         out << text << "\n";
//     }
// }

// Calculations_Energy_window::Calculations_Energy_window(QWidget *parent)
//     : QMainWindow(parent)
// {
//     debugLog("1 constructor start");

//     setupUi();

//     debugLog("2 setupUi finished");

//     setAttribute(Qt::WA_DeleteOnClose);

//     debugLog("3 constructor end");
// }

// void Calculations_Energy_window::moveSelectedRow(QTableWidget *table, int direction)
// {
//     if (!table) return;

//     int row = table->currentRow();
//     if (row < 0) return;

//     int targetRow = row + direction;
//     if (targetRow < 0 || targetRow >= table->rowCount()) return;

//     for (int col = 0; col < table->columnCount(); ++col) {
//         QTableWidgetItem *currentItem = table->takeItem(row, col);
//         QTableWidgetItem *targetItem = table->takeItem(targetRow, col);

//         table->setItem(row, col, targetItem);
//         table->setItem(targetRow, col, currentItem);
//     }

//     table->selectRow(targetRow);
//     table->setCurrentCell(targetRow, 0);
// }

// QTableWidget *Calculations_Energy_window::createUnitsTable()
// {
//     debugLog("createUnitsTable 1 entered");

//     QTableWidget *table = new QTableWidget(this);

//     debugLog("createUnitsTable 2 after new");

//     table->setColumnCount(2);
//     table->setRowCount(1);

//     debugLog("createUnitsTable 3 after size");

//     table->setItem(0, 0, new QTableWidgetItem("Тест"));
//     table->setItem(0, 1, new QTableWidgetItem("Тест"));

//     debugLog("createUnitsTable 4 end");

//     return table;
// }
// // QTableWidget *Calculations_Energy_window::createUnitsTable()//табличка верхняя
// // {
// //     QTableWidget *table = new QTableWidget(this);

// //     table->setColumnCount(2);
// //     table->setRowCount(3);

// //     table->setHorizontalHeaderLabels({
// //         "Название установки",
// //         "Комментарий"

// //     });

// //     table->setItem(0, 0, new QTableWidgetItem("Аппарат с газовой фазой"));
// //     table->setItem(0, 1, new QTableWidgetItem("Пример расчета E1"));

// //     table->setItem(1, 0, new QTableWidgetItem("Демонстрационная установка"));
// //     table->setItem(1, 1, new QTableWidgetItem("Расчет всех энергий"));

// //     table->setItem(2, 0, new QTableWidgetItem("Стадия компрессии (блок для расчета сценария риска)"));
// //     table->setItem(2, 1, new QTableWidgetItem("Пример расчета E1'"));

// //     table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
// //     table->verticalHeader()->setVisible(false);
// //     table->setMinimumHeight(230);
// //     table->horizontalHeader()->setFixedHeight(28);
// //     table->verticalHeader()->setDefaultSectionSize(26);
// //     table->setSelectionBehavior(QAbstractItemView::SelectRows);
// //     table->setSelectionMode(QAbstractItemView::SingleSelection);
// //     table->setEditTriggers(QAbstractItemView::NoEditTriggers);

// //     table->setStyleSheet(
// //         "QTableWidget::item:selected {"
// //         "background-color: black;"
// //         "color: white;"
// //         "}"
// //         );

// //     table->selectRow(0);

// //     return table;
// // }

// QTableWidget *Calculations_Energy_window::createBlocksTable()//нижняя табличка
// {
//     QTableWidget *table = new QTableWidget(this);

//     table->setColumnCount(9);
//     table->setRowCount(1);

//     table->setHorizontalHeaderLabels({
//         "Название блока",
//         "Коэф.\nуч. во\nвзрыве",
//         "Токсичные,\nвысокотокс\nичные в-ва",
//         "Кате-\nгория:",
//         "Полный\nпотен-циал",
//         "Приведен-\nная масса:",
//         "Относитель\nный\nпотенциал:",
//         "Радиус\nRo:",
//         "Оценка\nзон\nпораж."
//     });

//     table->setItem(0, 0, new QTableWidgetItem("Аппарат с газовой фазой"));
//     table->setItem(0, 1, new QTableWidgetItem("0,1"));

//     QTableWidgetItem *toxicItem = new QTableWidgetItem("");

//     toxicItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

//     toxicItem->setCheckState(Qt::Unchecked);

//     table->setItem(0, 2, toxicItem);
//     table->setItem(0, 3, new QTableWidgetItem("2"));
//     table->setItem(0, 4, new QTableWidgetItem("1,28E+08"));
//     table->setItem(0, 5, new QTableWidgetItem("2,78E+03"));
//     table->setItem(0, 6, new QTableWidgetItem("30,49"));
//     table->setItem(0, 7, new QTableWidgetItem("7,741"));

//     QTableWidgetItem *zoneItem = new QTableWidgetItem("");

//     zoneItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
//     zoneItem->setCheckState(Qt::Checked);
//     table->setItem(0, 8, zoneItem);

//     table->verticalHeader()->setVisible(false);
//     table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
//     table->horizontalHeader()->setFixedHeight(85);
//     table->verticalHeader()->setDefaultSectionSize(28);
//     table->setMinimumHeight(180);
//     table->setColumnWidth(0, 380);
//     table->setColumnWidth(1, 90);
//     table->setColumnWidth(2, 130);
//     table->setColumnWidth(3, 80);
//     table->setColumnWidth(4, 130);
//     table->setColumnWidth(5, 130);
//     table->setColumnWidth(6, 130);
//     table->setColumnWidth(7, 90);
//     table->setColumnWidth(8, 100);

//     table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
//     table->horizontalHeader()->setMinimumHeight(85);

//     table->setSelectionBehavior(QAbstractItemView::SelectRows);
//     table->setSelectionMode(QAbstractItemView::SingleSelection);
//     table->setEditTriggers(QAbstractItemView::NoEditTriggers);

//     table->setShowGrid(true);
//     table->setAlternatingRowColors(false);
//     table->setMinimumHeight(160);

//     return table;
// }
// void Calculations_Energy_window::setupUi()
// {
//     debugLog("setupUi 1 entered");

//     QWidget *central = new QWidget(this);
//     setCentralWidget(central);

//     debugLog("setupUi 2 central created");

//     setWindowTitle("Расчеты энергопотенциалов");
//     resize(1250, 700);

//     debugLog("setupUi 3 before createUnitsTable");

//     unitsTable = createUnitsTable();

//     debugLog("setupUi 4 after createUnitsTable");

//     QVBoxLayout *mainLayout = new QVBoxLayout(central);
//     mainLayout->addWidget(new QLabel("Установки :", this));
//     mainLayout->addWidget(unitsTable);

//     closeButton = new QPushButton("Закрыть", this);
//     mainLayout->addWidget(closeButton);

//     connect(closeButton, &QPushButton::clicked, this, &Calculations_Energy_window::close);

//     debugLog("setupUi 5 end");
// }
// // void Calculations_Energy_window::setupUi()
// // {
// //     QWidget *central = new QWidget(this);
// //     setCentralWidget(central);

// //     setWindowTitle("Расчеты энергопотенциалов");
// //     resize(1250, 700);

// //     unitsTable = createUnitsTable();
// //     blocksTable = createBlocksTable();

// //     QPushButton *unitEditButton = new QPushButton("Редактор", this);
// //     QPushButton *unitAddButton = new QPushButton("Добавить", this);
// //     QPushButton *unitDeleteButton = new QPushButton("Удалить", this);
// //     QPushButton *unitCopyButton = new QPushButton("Копировать", this);
// //     QPushButton *reportButton = new QPushButton("Отчёт", this);
// //     QPushButton *deletedButton = new QPushButton("Удаленные", this);

// //     QPushButton *unitUpButton = new QPushButton("⌃", this);
// //     QPushButton *unitDownButton = new QPushButton("⌄", this);

// //     QPushButton *blockEditButton = new QPushButton("Редактор", this);
// //     QPushButton *blockAddButton = new QPushButton("Добавить", this);
// //     QPushButton *blockDeleteButton = new QPushButton("Удалить", this);
// //     QPushButton *blockCopyButton = new QPushButton("Копировать", this);

// //     QPushButton *blockUpButton = new QPushButton("⌃", this);
// //     QPushButton *blockDownButton = new QPushButton("⌄", this);

// //     closeButton = new QPushButton("Закрыть", this);
// //     QPushButton *helpButton = new QPushButton("Помощь", this);

// //     QList<QPushButton*> buttons = {
// //         unitEditButton, unitAddButton, unitDeleteButton, unitCopyButton,
// //         reportButton, deletedButton, unitUpButton, unitDownButton,
// //         blockEditButton, blockAddButton, blockDeleteButton, blockCopyButton,
// //         blockUpButton, blockDownButton, closeButton, helpButton
// //     };

// //     for (QPushButton *button : buttons) {
// //         button->setMinimumHeight(34);
// //         button->setCursor(Qt::PointingHandCursor);
// //     }

// //     unitEditButton->setMinimumWidth(120);
// //     unitAddButton->setMinimumWidth(120);
// //     unitDeleteButton->setMinimumWidth(120);
// //     unitCopyButton->setMinimumWidth(120);
// //     reportButton->setMinimumWidth(120);
// //     deletedButton->setMinimumWidth(120);

// //     blockEditButton->setMinimumWidth(120);
// //     blockAddButton->setMinimumWidth(120);
// //     blockDeleteButton->setMinimumWidth(120);
// //     blockCopyButton->setMinimumWidth(120);

// //     closeButton->setMinimumWidth(120);
// //     helpButton->setMinimumWidth(120);

// //     QLabel *unitsLabel = new QLabel("Установки :", this);
// //     QLabel *blocksLabel = new QLabel("Блоки :", this);

// //     QFont labelFont;
// //     labelFont.setBold(true);
// //     unitsLabel->setFont(labelFont);
// //     blocksLabel->setFont(labelFont);

// //     QVBoxLayout *unitButtonsLayout = new QVBoxLayout;
// //     unitButtonsLayout->addWidget(unitEditButton);
// //     unitButtonsLayout->addWidget(unitAddButton);
// //     unitButtonsLayout->addWidget(unitDeleteButton);
// //     unitButtonsLayout->addWidget(unitCopyButton);
// //     unitButtonsLayout->addWidget(reportButton);
// //     unitButtonsLayout->addSpacing(15);
// //     unitButtonsLayout->addWidget(deletedButton);
// //     unitButtonsLayout->addStretch();

// //     QVBoxLayout *unitMoveButtonsLayout = new QVBoxLayout;
// //     unitMoveButtonsLayout->addSpacing(80);
// //     unitMoveButtonsLayout->addWidget(unitUpButton);
// //     unitMoveButtonsLayout->addWidget(unitDownButton);
// //     unitMoveButtonsLayout->addStretch();

// //     QHBoxLayout *unitsContentLayout = new QHBoxLayout;
// //     unitsContentLayout->addWidget(unitsTable, 1);
// //     unitsContentLayout->addLayout(unitButtonsLayout);
// //     unitsContentLayout->addLayout(unitMoveButtonsLayout);

// //     QVBoxLayout *topLayout = new QVBoxLayout;
// //     topLayout->addWidget(unitsLabel);
// //     topLayout->addLayout(unitsContentLayout);

// //     QHBoxLayout *blockButtonsLayout = new QHBoxLayout;
// //     blockButtonsLayout->addWidget(blockUpButton);
// //     blockButtonsLayout->addWidget(blockDownButton);
// //     blockButtonsLayout->addStretch();
// //     blockButtonsLayout->addWidget(blockEditButton);
// //     blockButtonsLayout->addWidget(blockAddButton);
// //     blockButtonsLayout->addWidget(blockDeleteButton);
// //     blockButtonsLayout->addWidget(blockCopyButton);
// //     blockButtonsLayout->addStretch();

// //     QVBoxLayout *blocksLayout = new QVBoxLayout;
// //     blocksLayout->addWidget(blocksLabel);
// //     blocksLayout->addWidget(blocksTable);
// //     blocksLayout->addLayout(blockButtonsLayout);

// //     QHBoxLayout *bottomLayout = new QHBoxLayout;
// //     bottomLayout->addStretch();
// //     bottomLayout->addWidget(closeButton);
// //     bottomLayout->addWidget(helpButton);

// //     QVBoxLayout *mainLayout = new QVBoxLayout(central);
// //     mainLayout->addLayout(topLayout);
// //     mainLayout->addLayout(blocksLayout);
// //     mainLayout->addStretch();
// //     mainLayout->addLayout(bottomLayout);

// //     connect(closeButton, &QPushButton::clicked, this, &Calculations_Energy_window::close);

// //     // connect(unitAddButton, &QPushButton::clicked, this, [this]() {
// //     //     int row = unitsTable->rowCount();
// //     //     unitsTable->insertRow(row);
// //     //     unitsTable->setItem(row, 0, new QTableWidgetItem("Новая установка"));
// //     //     unitsTable->setItem(row, 1, new QTableWidgetItem("Комментарий"));
// //     //     unitsTable->selectRow(row);
// //     // });

// //     // connect(unitDeleteButton, &QPushButton::clicked, this, [this]() {
// //     //     int row = unitsTable->currentRow();
// //     //     if (row >= 0) {
// //     //         unitsTable->removeRow(row);
// //     //     }
// //     // });

// //     // connect(unitCopyButton, &QPushButton::clicked, this, [this]() {
// //     //     int row = unitsTable->currentRow();
// //     //     if (row < 0) return;

// //     //     int newRow = unitsTable->rowCount();
// //     //     unitsTable->insertRow(newRow);

// //     //     for (int col = 0; col < unitsTable->columnCount(); ++col) {
// //     //         QTableWidgetItem *oldItem = unitsTable->item(row, col);
// //     //         if (oldItem) {
// //     //             unitsTable->setItem(newRow, col, oldItem->clone());
// //     //         }
// //     //     }

// //     //     unitsTable->selectRow(newRow);
// //     // });

// //     // connect(blockAddButton, &QPushButton::clicked, this, [this]() {
// //     //     int row = blocksTable->rowCount();
// //     //     blocksTable->insertRow(row);

// //     //     blocksTable->setItem(row, 0, new QTableWidgetItem("Новый блок"));
// //     //     blocksTable->setItem(row, 1, new QTableWidgetItem("0,1"));

// //     //     QTableWidgetItem *toxicItem = new QTableWidgetItem();
// //     //     toxicItem->setFlags(toxicItem->flags() | Qt::ItemIsUserCheckable);
// //     //     toxicItem->setCheckState(Qt::Unchecked);
// //     //     blocksTable->setItem(row, 2, toxicItem);

// //     //     blocksTable->setItem(row, 3, new QTableWidgetItem("2"));
// //     //     blocksTable->setItem(row, 4, new QTableWidgetItem("0"));
// //     //     blocksTable->setItem(row, 5, new QTableWidgetItem("0"));
// //     //     blocksTable->setItem(row, 6, new QTableWidgetItem("0"));
// //     //     blocksTable->setItem(row, 7, new QTableWidgetItem("0"));

// //     //     QTableWidgetItem *zoneItem = new QTableWidgetItem();
// //     //     zoneItem->setFlags(zoneItem->flags() | Qt::ItemIsUserCheckable);
// //     //     zoneItem->setCheckState(Qt::Unchecked);
// //     //     blocksTable->setItem(row, 8, zoneItem);

// //     //     blocksTable->selectRow(row);
// //     // });

// //     // connect(blockDeleteButton, &QPushButton::clicked, this, [this]() {
// //     //     int row = blocksTable->currentRow();
// //     //     if (row >= 0) {
// //     //         blocksTable->removeRow(row);
// //     //     }
// //     // });

// //     // connect(blockCopyButton, &QPushButton::clicked, this, [this]() {
// //     //     int row = blocksTable->currentRow();
// //     //     if (row < 0) return;

// //     //     int newRow = blocksTable->rowCount();
// //     //     blocksTable->insertRow(newRow);

// //     //     for (int col = 0; col < blocksTable->columnCount(); ++col) {
// //     //         QTableWidgetItem *oldItem = blocksTable->item(row, col);
// //     //         if (oldItem) {
// //     //             blocksTable->setItem(newRow, col, oldItem->clone());
// //     //         }
// //     //     }

// //     //     blocksTable->selectRow(newRow);
// //     // });
// //     // //движение по таблицам вверх и вниз
// //     // connect(unitUpButton, &QPushButton::clicked, this, [this]() {
// //     //     moveSelectedRow(unitsTable, -1);
// //     // });

// //     // connect(unitDownButton, &QPushButton::clicked, this, [this]() {
// //     //     moveSelectedRow(unitsTable, 1);
// //     // });

// //     // connect(blockUpButton, &QPushButton::clicked, this, [this]() {
// //     //     moveSelectedRow(blocksTable, -1);
// //     // });

// //     // connect(blockDownButton, &QPushButton::clicked, this, [this]() {
// //     //     moveSelectedRow(blocksTable, 1);
// //     // });

// //     setStyleSheet(
// //         "QMainWindow, QWidget {"
// //         "    background-color: #efefef;"
// //         "    font-family: Arial;"
// //         "    font-size: 14px;"
// //         "}"

// //         "QLabel {"
// //         "    color: black;"
// //         "}"

// //         "QTableWidget {"
// //         "    background-color: white;"
// //         "    alternate-background-color: white;"
// //         "    gridline-color: #c8c8c8;"
// //         "    color: black;"
// //         "    border: 1px solid #b0b0b0;"
// //         "    selection-background-color: black;"
// //         "    selection-color: white;"
// //         "}"

// //         "QHeaderView::section {"
// //         "    background-color: #f5f5f5;"
// //         "    color: black;"
// //         "    border: 1px solid #c0c0c0;"
// //         "    padding: 4px;"
// //         "    font-weight: bold;"
// //         "}"

// //         "QTableWidget::item {"
// //         "    padding: 2px;"
// //         "}"

// //         "QTableWidget::item:selected {"
// //         "    background-color: black;"
// //         "    color: white;"
// //         "}"

// //         "QPushButton {"
// //         "    background-color: #f2f2f2;"
// //         "    color: black;"
// //         "    border: 1px solid #9f9f9f;"
// //         "    padding: 5px 14px;"
// //         "}"

// //         "QPushButton:hover {"
// //         "    background-color: #e1e1e1;"
// //         "}"

// //         "QPushButton:pressed {"
// //         "    background-color: #d0d0d0;"
// //         "}"

// //         "QPushButton:disabled {"
// //         "    color: #888888;"
// //         "    background-color: #dddddd;"
// //         "}"

// //         "QPushButton[text=\"Закрыть\"], QPushButton[text=\"Отчёт\"] {"
// //         "    font-weight: bold;"
// //         "}"
// //         );
// // }

#include "calculations_of_energy_potentials_window.h"

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>

Calculations_Energy_window::Calculations_Energy_window(QWidget *parent)
    : QMainWindow(parent)
{
    buildInterface();
    setAttribute(Qt::WA_DeleteOnClose);
}

QTableWidget *Calculations_Energy_window::createUnitsTable()
{
    QTableWidget *table = new QTableWidget(this);

    table->setColumnCount(2);
    table->setRowCount(3);

    table->setHorizontalHeaderLabels({
        "Название установки",
        "Комментарий"
    });

    table->setItem(0, 0, new QTableWidgetItem("Аппарат с газовой фазой"));
    table->setItem(0, 1, new QTableWidgetItem("Пример расчета E1"));

    table->setItem(1, 0, new QTableWidgetItem("Демонстрационная установка"));
    table->setItem(1, 1, new QTableWidgetItem("Расчет всех энергий"));

    table->setItem(2, 0, new QTableWidgetItem("Стадия компрессии"));
    table->setItem(2, 1, new QTableWidgetItem("Пример расчета E1"));

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->selectRow(0);

    return table;
}

QTableWidget *Calculations_Energy_window::createBlocksTable()
{
    QTableWidget *table = new QTableWidget(this);

    table->setColumnCount(9);
    table->setRowCount(1);

    table->setHorizontalHeaderLabels({
        "Название блока",
        "Коэф.\nуч. во\nвзрыве",
        "Токсичные\nв-ва",
        "Категория",
        "Полный\nпотенциал",
        "Приведенная\nмасса",
        "Относительный\nпотенциал",
        "Радиус\nRo",
        "Оценка\nзон"
    });

    table->setItem(0, 0, new QTableWidgetItem("Аппарат с газовой фазой"));
    table->setItem(0, 1, new QTableWidgetItem("0,1"));
    table->setItem(0, 2, new QTableWidgetItem("Нет"));
    table->setItem(0, 3, new QTableWidgetItem("2"));
    table->setItem(0, 4, new QTableWidgetItem("1,28E+08"));
    table->setItem(0, 5, new QTableWidgetItem("2,78E+03"));
    table->setItem(0, 6, new QTableWidgetItem("30,49"));
    table->setItem(0, 7, new QTableWidgetItem("7,741"));
    table->setItem(0, 8, new QTableWidgetItem("Да"));

    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    table->setColumnWidth(0, 380);
    table->setColumnWidth(1, 90);
    table->setColumnWidth(2, 120);
    table->setColumnWidth(3, 90);
    table->setColumnWidth(4, 130);
    table->setColumnWidth(5, 130);
    table->setColumnWidth(6, 150);
    table->setColumnWidth(7, 90);
    table->setColumnWidth(8, 90);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->selectRow(0);

    return table;
}

void Calculations_Energy_window::buildInterface()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setWindowTitle("Расчеты энергопотенциалов");
    resize(1250, 700);

    QLabel *unitsLabel = new QLabel("Установки :", this);
    QLabel *blocksLabel = new QLabel("Блоки :", this);

    unitsTable = createUnitsTable();
    blocksTable = createBlocksTable();

    closeButton = new QPushButton("Закрыть", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    mainLayout->addWidget(unitsLabel);
    mainLayout->addWidget(unitsTable);

    mainLayout->addWidget(blocksLabel);
    mainLayout->addWidget(blocksTable);

    mainLayout->addStretch();
    mainLayout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &Calculations_Energy_window::close);
}