#ifndef CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H
#define CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H

#include <QMainWindow>

class QTableWidget;
class QPushButton;

class Calculations_Energy_window : public QMainWindow
{
public:
    explicit Calculations_Energy_window(QWidget *parent = nullptr);
    ~Calculations_Energy_window() override = default;

private:
    QTableWidget *unitsTable = nullptr;
    QTableWidget *blocksTable = nullptr;

    QPushButton *closeButton = nullptr;

    void buildInterface();

    QTableWidget *createUnitsTable();
    QTableWidget *createBlocksTable();
};

#endif