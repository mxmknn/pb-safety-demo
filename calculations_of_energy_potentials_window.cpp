#include "calculations_of_energy_potentials_window.h"
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QSplitter>
#include <QFont>

Calculations_Energy_window::Calculations_Energy_window(QWidget *parent) //конструктор окна
    : QMainWindow(parent)
{
    setupUi();
    // setupTables();
    fillTestData();
    setAttribute(Qt::WA_DeleteOnClose);
}

Calculations_Energy_window::~Calculations_Energy_window()
{
}

QTableWidget *Calculations_Energy_window::createUnitsTable()//табличка верхняя
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

    table->setItem(2, 0, new QTableWidgetItem("Стадия компрессии (блок для расчета сценария риска)"));
    table->setItem(2, 1, new QTableWidgetItem("Пример расчета E1'"));

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table->setStyleSheet(
        "QTableWidget::item:selected {"
        "background-color: black;"
        "color: white;"
        "}"
        );

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
        "Токсичные,\nвысокотокс\nичные в-ва",
        "Кате-\nгория:",
        "Полный\nпотен-циал",
        "Приведен-\nная масса:",
        "Относитель\nный\nпотенциал:",
        "Радиус\nRo:",
        "Оценка\nзон\nпораж."
    });

    table->setItem(0, 0, new QTableWidgetItem("Аппарат с газовой фазой"));
    table->setItem(0, 1, new QTableWidgetItem("0,1"));

    QTableWidgetItem *toxicItem = new QTableWidgetItem();
    toxicItem->setFlags(toxicItem->flags() | Qt::ItemIsUserCheckable);
    toxicItem->setCheckState(Qt::Unchecked);
    table->setItem(0, 2, toxicItem);

    table->setItem(0, 3, new QTableWidgetItem("2"));
    table->setItem(0, 4, new QTableWidgetItem("1,28E+08"));
    table->setItem(0, 5, new QTableWidgetItem("2,78E+03"));
    table->setItem(0, 6, new QTableWidgetItem("30,49"));
    table->setItem(0, 7, new QTableWidgetItem("7,741"));

    QTableWidgetItem *zoneItem = new QTableWidgetItem();
    zoneItem->setFlags(zoneItem->flags() | Qt::ItemIsUserCheckable);
    zoneItem->setCheckState(Qt::Checked);
    table->setItem(0, 8, zoneItem);

    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    table->setColumnWidth(0, 380);
    table->setColumnWidth(1, 90);
    table->setColumnWidth(2, 130);
    table->setColumnWidth(3, 80);
    table->setColumnWidth(4, 130);
    table->setColumnWidth(5, 130);
    table->setColumnWidth(6, 130);
    table->setColumnWidth(7, 90);
    table->setColumnWidth(8, 100);

    table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table->horizontalHeader()->setMinimumHeight(85);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table->setShowGrid(true);
    table->setAlternatingRowColors(false);

    table->setStyleSheet(
        "QTableWidget {"
        "    background-color: white;"
        "    gridline-color: #bfbfbf;"
        "    font-size: 14px;"
        "}"
        "QHeaderView::section {"
        "    background-color: #eeeeee;"
        "    color: black;"
        "    border: 1px solid #a8a8a8;"
        "    padding: 3px;"
        "    font-size: 14px;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: white;"
        "    color: black;"
        "}"
        );

    table->setMinimumHeight(160);

    return table;
}

void Calculations_Energy_window::setupUi() // внешка окна
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setWindowTitle("Расчеты энергопотенциалов");
    resize(1200, 750);

    QLabel *titleLabel = new QLabel("Расчеты энергопотенциалов и категорий взрывоопасности", this);

    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    unitsTable = createUnitsTable(); //вызываем верхнюю табличку
    blocksTable = createBlocksTable(); //вызываем нижнюю табличку

    QGroupBox *unitsGroup = new QGroupBox("Установки", this);
    QVBoxLayout *unitsLayout = new QVBoxLayout;
    unitsLayout->addWidget(unitsTable);
    unitsGroup->setLayout(unitsLayout);

    QGroupBox *blocksGroup = new QGroupBox("Блоки", this);
    QVBoxLayout *blocksLayout = new QVBoxLayout;
    blocksLayout->addWidget(blocksTable);
    blocksGroup->setLayout(blocksLayout);

    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(unitsGroup);
    splitter->addWidget(blocksGroup);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    backButton = new QPushButton("Назад в меню", this);
    closeButton = new QPushButton("Закрыть", this);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(backButton);
    bottomLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(splitter);
    mainLayout->addLayout(bottomLayout);

    connect(closeButton, &QPushButton::clicked, this, &Calculations_Energy_window::close);

    connect(backButton, &QPushButton::clicked, this, [this]() {
        this->close();

    });
}

void Calculations_Energy_window::fillTestData()
{
    unitsTable->setRowCount(2);

    unitsTable->setItem(0, 0, new QTableWidgetItem("1"));
    unitsTable->setItem(0, 1, new QTableWidgetItem("Установка подготовки газа"));
    unitsTable->setItem(0, 2, new QTableWidgetItem("Основная установка"));

    unitsTable->setItem(1, 0, new QTableWidgetItem("2"));
    unitsTable->setItem(1, 1, new QTableWidgetItem("Насосная станция"));
    unitsTable->setItem(1, 2, new QTableWidgetItem("Перекачка продукта"));

    blocksTable->setRowCount(2);

    blocksTable->setItem(0, 0, new QTableWidgetItem("1"));
    blocksTable->setItem(0, 1, new QTableWidgetItem("Блок сепарации"));
    blocksTable->setItem(0, 2, new QTableWidgetItem("Описание блока"));
    blocksTable->setItem(0, 3, new QTableWidgetItem("1.0"));
    blocksTable->setItem(0, 4, new QTableWidgetItem("Да"));
    blocksTable->setItem(0, 5, new QTableWidgetItem("920000"));
    blocksTable->setItem(0, 6, new QTableWidgetItem("20.00"));
    blocksTable->setItem(0, 7, new QTableWidgetItem("5.90"));
    blocksTable->setItem(0, 8, new QTableWidgetItem("II"));

    blocksTable->setItem(1, 0, new QTableWidgetItem("2"));
    blocksTable->setItem(1, 1, new QTableWidgetItem("Блок насосов"));
    blocksTable->setItem(1, 2, new QTableWidgetItem("Насосное оборудование"));
    blocksTable->setItem(1, 3, new QTableWidgetItem("1.2"));
    blocksTable->setItem(1, 4, new QTableWidgetItem("Нет"));
    blocksTable->setItem(1, 5, new QTableWidgetItem("540000"));
    blocksTable->setItem(1, 6, new QTableWidgetItem("11.74"));
    blocksTable->setItem(1, 7, new QTableWidgetItem("4.93"));
    blocksTable->setItem(1, 8, new QTableWidgetItem("III"));
}

