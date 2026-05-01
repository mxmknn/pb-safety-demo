#include "calculations_of_energy_potentials_window.h"
#include <QHeaderView>
#include <QAbstractItemView>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>

Calculations_Energy_window::Calculations_Energy_window(QWidget *parent)
    : QMainWindow(parent)
{
    buildInterface();
    setAttribute(Qt::WA_DeleteOnClose);
}

void Calculations_Energy_window::buildInterface()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setWindowTitle("Расчеты энергопотенциалов");
    resize(800, 500);

    QLabel *label = new QLabel("Установки:", this);

    unitsTable = new QTableWidget(this);
    unitsTable->setColumnCount(2);
    unitsTable->setRowCount(3);
    unitsTable->setItem(0, 0, new QTableWidgetItem("Аппарат с газовой фазой"));
    unitsTable->setItem(0, 1, new QTableWidgetItem("Пример расчета E1"));

    unitsTable->setItem(1, 0, new QTableWidgetItem("Демонстрационная установка"));
    unitsTable->setItem(1, 1, new QTableWidgetItem("Расчет всех энергий"));

    unitsTable->setItem(2, 0, new QTableWidgetItem("Стадия компрессии"));
    unitsTable->setItem(2, 1, new QTableWidgetItem("Пример расчета E1"));
    closeButton = new QPushButton("Закрыть", this);
    unitsTable->verticalHeader()->setVisible(false);
    unitsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    unitsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    unitsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->addWidget(label);
    mainLayout->addWidget(unitsTable);
    mainLayout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &Calculations_Energy_window::close);

    QLabel *blocksLabel = new QLabel("Блоки :", this);

    QTableWidget *blocksTable = new QTableWidget(this);
    blocksTable->setColumnCount(9);
    blocksTable->setRowCount(1);

    blocksTable->setHorizontalHeaderLabels({
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

    blocksTable->setItem(0, 0, new QTableWidgetItem("Аппарат с газовой фазой"));
    blocksTable->setItem(0, 1, new QTableWidgetItem("0,1"));
    blocksTable->setItem(0, 2, new QTableWidgetItem("Нет"));
    blocksTable->setItem(0, 3, new QTableWidgetItem("2"));
    blocksTable->setItem(0, 4, new QTableWidgetItem("1,28E+08"));
    blocksTable->setItem(0, 5, new QTableWidgetItem("2,78E+03"));
    blocksTable->setItem(0, 6, new QTableWidgetItem("30,49"));
    blocksTable->setItem(0, 7, new QTableWidgetItem("7,741"));
    blocksTable->setItem(0, 8, new QTableWidgetItem("Да"));

    blocksTable->verticalHeader()->setVisible(false);
    blocksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    blocksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    blocksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(blocksLabel);
    mainLayout->addWidget(blocksTable);
}