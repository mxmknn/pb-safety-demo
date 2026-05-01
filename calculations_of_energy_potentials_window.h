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
    QTableWidget *unitsTable;
    QTableWidget *blocksTable;

    QPushButton *backButton;
    QPushButton *closeButton;

    void setupUi();
    // void setupTables();
    void fillTestData();

    QTableWidget *createUnitsTable();
    QTableWidget *createBlocksTable();
};

#endif