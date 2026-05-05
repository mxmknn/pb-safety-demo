#include "mainwindow.h"

#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const QString databasePath = QStringLiteral("/Users/mxmknn/Downloads/PVB_DB.db");
    QFileInfo databaseFile(databasePath);
    if (!databaseFile.exists()) {
        QMessageBox::critical(nullptr,
                              QStringLiteral("Ошибка базы данных"),
                              QStringLiteral("Файл БД не найден:\n%1").arg(databasePath));
        return 1;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    db.setDatabaseName(databasePath);
    if (!db.open()) {
        QMessageBox::critical(nullptr,
                              QStringLiteral("Ошибка базы данных"),
                              QStringLiteral("Не удалось открыть БД:\n%1\n\n%2")
                                  .arg(databasePath, db.lastError().text()));
        return 1;
    }

    QSqlQuery pragmaQuery(db);
    if (!pragmaQuery.exec(QStringLiteral("PRAGMA foreign_keys = ON;"))) {
        QMessageBox::critical(nullptr,
                              QStringLiteral("Ошибка базы данных"),
                              QStringLiteral("Не удалось включить контроль внешних ключей:\n%1")
                                  .arg(pragmaQuery.lastError().text()));
        return 1;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
