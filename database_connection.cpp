#include "database_connection.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

namespace {
constexpr auto kConnectionName = "pb_safety_main";
constexpr auto kDatabaseFileName = "PVB_DB.db";

QStringList candidatePaths()
{
    QStringList paths;

    const QString fromEnv = qEnvironmentVariable("PVB_DB_PATH");
    if (!fromEnv.isEmpty()) {
        paths << fromEnv;
    }

    paths << QDir::current().filePath(QString::fromLatin1(kDatabaseFileName));
    paths << QCoreApplication::applicationDirPath() + QDir::separator() + QString::fromLatin1(kDatabaseFileName);

    const QString downloadsDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (!downloadsDir.isEmpty()) {
        paths << downloadsDir + QDir::separator() + QString::fromLatin1(kDatabaseFileName);
    }

    const QString homeDir = QDir::homePath();
    if (!homeDir.isEmpty()) {
        paths << homeDir + QDir::separator() + QStringLiteral("Downloads") + QDir::separator() + QString::fromLatin1(kDatabaseFileName);
    }

    paths.removeDuplicates();
    return paths;
}

QString resolveExistingDatabasePath(QString *diagnostic)
{
    QStringList tried;
    const QStringList candidates = candidatePaths();

    for (const QString &path : candidates) {
        if (path.isEmpty()) {
            continue;
        }

        tried << path;
        QFileInfo fileInfo(path);
        if (fileInfo.exists() && fileInfo.isFile() && fileInfo.isReadable()) {
            if (diagnostic) {
                *diagnostic = QStringLiteral("Найден файл БД: %1").arg(path);
            }
            return path;
        }
    }

    if (diagnostic) {
        *diagnostic = QStringLiteral("Проверены пути:\n- %1").arg(tried.join(QStringLiteral("\n- ")));
    }

    return {};
}
}

DatabaseInitResult initializeDatabaseConnection()
{
    DatabaseInitResult result;

    QString diagnostic;
    const QString databasePath = resolveExistingDatabasePath(&diagnostic);
    if (databasePath.isEmpty()) {
        result.errorMessage = QStringLiteral("Файл БД PVB_DB.db не найден.\n\n%1\n\n"
                                             "Можно указать путь через переменную окружения PVB_DB_PATH.")
                                  .arg(diagnostic);
        return result;
    }

    if (QSqlDatabase::contains(QString::fromLatin1(kConnectionName))) {
        QSqlDatabase::removeDatabase(QString::fromLatin1(kConnectionName));
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QString::fromLatin1(kConnectionName));
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        result.errorMessage = QStringLiteral("Не удалось открыть БД:\n%1\n\n%2")
                                  .arg(databasePath, db.lastError().text());
        return result;
    }

    QSqlQuery pragmaQuery(db);
    if (!pragmaQuery.exec(QStringLiteral("PRAGMA foreign_keys = ON;"))) {
        result.errorMessage = QStringLiteral("Не удалось включить внешний ключи:\n%1")
                                  .arg(pragmaQuery.lastError().text());
        return result;
    }

    result.success = true;
    result.path = databasePath;
    return result;
}
