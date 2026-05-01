#include "calculations_of_energy_potentials_window.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QAbstractItemView>

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
}

void Calculations_Energy_window::buildInterface()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setWindowTitle("Расчеты энергопотенциалов");
    resize(1000, 650);

    QLabel *unitsLabel = new QLabel("Установки :", this);
    QLabel *blocksLabel = new QLabel("Блоки :", this);

    createUnitsTable();
    createBlocksTable();

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