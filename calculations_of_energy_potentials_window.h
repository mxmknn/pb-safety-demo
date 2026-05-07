#ifndef CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H
#define CALCULATIONS_OF_ENERGY_POTENTIALS_WINDOW_H

#include <QMainWindow>
#include <QString>

class QPushButton;
class QSqlDatabase;
class QSqlQuery;
class QStandardItemModel;
class QTableView;

#ifdef PB_SAFETY_HAS_LIMEREPORT
namespace LimeReport {
class ReportEngine;
}
#endif

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

    enum UnitColumns {
        UnitNameColumn = 0,
        UnitCommentColumn,
        UnitColumnCount
    };

    enum BlockColumns {
        BlockNameColumn = 0,
        BlockKEruptionColumn,
        BlockClassColumn,
        BlockCategoryColumn,
        BlockTotalEnergyColumn,
        BlockReducedMassColumn,
        BlockRelativePotentialColumn,
        BlockRadiusColumn,
        BlockPrintRColumn,
        BlockColumnCount
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

#ifdef PB_SAFETY_HAS_LIMEREPORT
    LimeReport::ReportEngine *reportEngine = nullptr;
#endif

    void buildInterface();
    void createUnitsTable();
    void createBlocksTable();
    QPushButton *createButton(const QString &text, int width = 110);

    void onUnitsSelectionChanged();
    void onBlocksSelectionChanged();

    void refreshUnits(qint64 preferredUnitId = -1);
    void refreshBlocks(qint64 preferredBlockId = -1);
    void clearBlocks();
    void updateActionStates();

    bool ensureDatabaseReady(const QString &actionName) const;
    bool executeQuery(QSqlQuery &query, const QString &actionName) const;
    bool beginTransaction(const QString &actionName, QSqlDatabase &db) const;
    bool commitTransaction(const QString &actionName, QSqlDatabase &db) const;
    void rollbackTransaction(QSqlDatabase &db) const;

    int findRowById(const QStandardItemModel *model, qint64 id) const;
    qint64 selectedId(const QTableView *table, const QStandardItemModel *model) const;
    int selectedIndexValue(const QTableView *table, const QStandardItemModel *model) const;
    void selectRow(QTableView *table, QStandardItemModel *model, int row) const;

    qint64 selectedUnitId() const;
    qint64 selectedBlockId() const;
    int selectedUnitIndexValue() const;
    int selectedBlockIndexValue() const;

    bool nextUnitIndex(QSqlDatabase &db, int *value) const;
    bool nextBlockIndex(QSqlDatabase &db, qint64 unitId, int *value) const;
    bool swapIndexes(const QString &tableName,
                     qint64 currentId,
                     int currentIndex,
                     qint64 neighborId,
                     int neighborIndex,
                     const QString &actionName);

    void showNotImplemented(const QString &actionName);
    void generateUnitReport();
    QStandardItemModel *buildUnitReportModel();

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
