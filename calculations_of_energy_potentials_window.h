#ifndef CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H
#define CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H

#include <QMainWindow>

class QPushButton;
class QTableView;
class QStandardItemModel;

class Calculations_Energy_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculations_Energy_window(QWidget *parent = nullptr);
    ~Calculations_Energy_window() override = default;

private:
    enum ItemRoles {
        IdRole = Qt::UserRole + 1,
        IndexRole
    };

    QTableView *unitsTable = nullptr;
    QTableView *blocksTable = nullptr;

    QStandardItemModel *unitsModel = nullptr;
    QStandardItemModel *blocksModel = nullptr;

    QPushButton *unitAddButton = nullptr;
    QPushButton *unitDeleteButton = nullptr;
    QPushButton *unitCopyButton = nullptr;
    QPushButton *unitDeletedButton = nullptr;
    QPushButton *unitUpButton = nullptr;
    QPushButton *unitDownButton = nullptr;

    QPushButton *blockAddButton = nullptr;
    QPushButton *blockDeleteButton = nullptr;
    QPushButton *blockCopyButton = nullptr;
    QPushButton *blockUpButton = nullptr;
    QPushButton *blockDownButton = nullptr;

    QPushButton *closeButton = nullptr;
    bool showDeletedUnits = false;

    void buildInterface();
    void createUnitsTable();
    void createBlocksTable();
    QPushButton *createButton(const QString &text, int width = 110);

    void refreshUnits(qint64 preferredUnitId = -1);
    void refreshBlocks(qint64 preferredBlockId = -1);
    void updateActionStates();

    qint64 selectedUnitId() const;
    qint64 selectedBlockId() const;
    int selectedUnitIndexValue() const;
    int selectedBlockIndexValue() const;

    int nextUnitIndex() const;
    int nextBlockIndex(qint64 unitId) const;
    bool executeQuery(class QSqlQuery &query, const QString &actionName);
    void showNotImplemented(const QString &actionName);

    void addUnit();
    void deleteSelectedUnit();
    void copySelectedUnit();
    void moveSelectedUnit(int direction);
    void toggleDeletedUnitsMode();

    void addBlock();
    void deleteSelectedBlock();
    void copySelectedBlock();
    void moveSelectedBlock(int direction);
};

#endif
