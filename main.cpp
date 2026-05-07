#include "database_connection.h"
#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>
#include <exception>

int main(int argc, char *argv[])
{
    try {
        QApplication app(argc, argv);
        QCoreApplication::setOrganizationName(QStringLiteral("pb-safety"));
        QCoreApplication::setApplicationName(QStringLiteral("pb-safety"));

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
    } catch (const std::exception &exception) {
        QMessageBox::critical(nullptr,
                              QStringLiteral("Критическая ошибка"),
                              QStringLiteral("Приложение завершилось из-за исключения C++:\n%1")
                                  .arg(QString::fromUtf8(exception.what())));
    } catch (...) {
        QMessageBox::critical(nullptr,
                              QStringLiteral("Критическая ошибка"),
                              QStringLiteral("Приложение завершилось из-за неизвестного исключения."));
    }

    return 2;
}
