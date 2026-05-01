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
}