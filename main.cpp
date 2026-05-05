#include "database_connection.h"
#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const DatabaseInitResult dbInit = initializeDatabaseConnection();
    if (!dbInit.success) {
        QMessageBox::critical(nullptr,
                              QStringLiteral("Ошибка базы данных"),
                              dbInit.errorMessage);
        return 1;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
