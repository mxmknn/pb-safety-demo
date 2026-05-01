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