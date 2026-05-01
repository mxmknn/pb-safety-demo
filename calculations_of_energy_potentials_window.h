#ifndef CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H
#define CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H

#include <QMainWindow>

class QPushButton;
class QTableView;
class QStandardItemModel;

class Calculations_Energy_window : public QMainWindow
{
public:
    explicit Calculations_Energy_window(QWidget *parent = nullptr);
    ~Calculations_Energy_window() override = default;

private:
    QPushButton *closeButton = nullptr;

    QTableView *unitsTable = nullptr;
    QTableView *blocksTable = nullptr;

    QStandardItemModel *unitsModel = nullptr;
    QStandardItemModel *blocksModel = nullptr;

    void buildInterface();
    void createUnitsTable();
    void createBlocksTable();

    void addUnitRow();
    void deleteSelectedUnitRow();
    void copySelectedUnitRow();

    void addBlockRow();
    void deleteSelectedBlockRow();
    void copySelectedBlockRow();

    void moveSelectedRow(QTableView *table, QStandardItemModel *model, int direction);
};

#endif