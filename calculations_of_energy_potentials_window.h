#ifndef CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H
#define CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H

#include <QMainWindow>

class QTableWidget;
class QPushButton;

class Calculations_Energy_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculations_Energy_window(QWidget *parent = nullptr);
    ~Calculations_Energy_window();

private:
    QTableWidget *unitsTable = nullptr;
    QTableWidget *blocksTable = nullptr;

    QPushButton *backButton = nullptr;
    QPushButton *closeButton = nullptr;

    void setupUi();

    QTableWidget *createUnitsTable();
    QTableWidget *createBlocksTable();

    void moveSelectedRow(QTableWidget *table, int direction);
};

#endif