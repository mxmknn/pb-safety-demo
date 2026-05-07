#include "database_connection.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QStandardPaths>

namespace {
constexpr auto kConnectionName = "pb_safety_main";
constexpr auto kDatabaseFileName = "PVB_DB.db";

QString databaseFileName()
{
    return QString::fromLatin1(kDatabaseFileName);
}

void appendUniquePath(QStringList *paths, const QString &candidatePath)
{
    if (!paths || candidatePath.isEmpty()) {
        return;
    }

    const QString normalized = QDir::cleanPath(QDir::fromNativeSeparators(candidatePath));
    if (!paths->contains(normalized)) {
        paths->append(normalized);
    }
}

void appendDatabaseInDirectory(QStringList *paths, const QString &directoryPath)
{
    if (directoryPath.isEmpty()) {
        return;
    }

    const QDir directory(directoryPath);
    appendUniquePath(paths, directory.filePath(databaseFileName()));
}

void appendParentDirectoryCandidates(QStringList *paths, const QString &startDirectoryPath, int levelsUp)
{
    if (startDirectoryPath.isEmpty() || levelsUp < 0) {
        return;
    }

    QDir directory(startDirectoryPath);
    for (int level = 0; level <= levelsUp; ++level) {
        appendDatabaseInDirectory(paths, directory.absolutePath());
        if (!directory.cdUp()) {
            break;
        }
    }
}

QStringList candidatePaths() // ищем бд
{
    QStringList paths;

    const QString fromEnv = qEnvironmentVariable("PVB_DB_PATH");
    if (!fromEnv.isEmpty()) {
        QFileInfo envInfo(fromEnv);
        if (envInfo.isDir()) {
            appendDatabaseInDirectory(&paths, envInfo.absoluteFilePath());
        } else {
            appendUniquePath(&paths, envInfo.absoluteFilePath());
        }
    }

    appendParentDirectoryCandidates(&paths, QDir::currentPath(), 3);
    appendParentDirectoryCandidates(&paths, QCoreApplication::applicationDirPath(), 5);

    const auto appendStandardLocation = [&paths](QStandardPaths::StandardLocation location) {
        const QStringList locations = QStandardPaths::standardLocations(location);
        for (const QString &entry : locations) {
            appendDatabaseInDirectory(&paths, entry);
        }
    };

    appendStandardLocation(QStandardPaths::AppLocalDataLocation);
    appendStandardLocation(QStandardPaths::AppDataLocation);
    appendStandardLocation(QStandardPaths::DocumentsLocation);
    appendStandardLocation(QStandardPaths::DownloadLocation);

    const QString homeDirPath = QDir::homePath();
    if (!homeDirPath.isEmpty()) {
        appendDatabaseInDirectory(&paths, homeDirPath);
        appendDatabaseInDirectory(&paths, homeDirPath + QDir::separator() + QStringLiteral("Downloads"));
        appendDatabaseInDirectory(&paths, homeDirPath + QDir::separator() + QStringLiteral("Documents"));
    }

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

    if (!QSqlDatabase::isDriverAvailable(QStringLiteral("QSQLITE"))) {
        result.errorMessage = QStringLiteral(
            "Qt-драйвер SQLite (QSQLITE) недоступен.\n\n"
            "Доступные драйверы: %1\n\n"
            "Пути поиска плагинов Qt:\n- %2")
                                  .arg(QSqlDatabase::drivers().join(QStringLiteral(", ")),
                                       QCoreApplication::libraryPaths().join(QStringLiteral("\n- ")));
        return result;
    }

    QString diagnostic;
    const QString databasePath = resolveExistingDatabasePath(&diagnostic);
    if (databasePath.isEmpty()) {
        result.errorMessage = QStringLiteral("Файл БД PVB_DB.db не найден.\n\n%1\n\n"
                                             "Можно указать путь через переменную окружения PVB_DB_PATH.")
                                  .arg(diagnostic);
        return result;
    }

    const QString connectionName = QString::fromLatin1(kConnectionName);
    if (QSqlDatabase::contains(connectionName)) {
        {
            QSqlDatabase oldConnection = QSqlDatabase::database(connectionName, false);
            if (oldConnection.isValid() && oldConnection.isOpen()) {
                oldConnection.close();
            }
        }
        QSqlDatabase::removeDatabase(connectionName);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName);
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        result.errorMessage = QStringLiteral("Не удалось открыть БД:\n%1\n\n%2")
                                  .arg(databasePath, db.lastError().text());
        return result;
    }

    QSqlQuery pragmaQuery(db);
    if (!pragmaQuery.exec(QStringLiteral("PRAGMA foreign_keys = ON;"))) {
        result.errorMessage = QStringLiteral("Не удалось включить внешние ключи:\n%1")
                                  .arg(pragmaQuery.lastError().text());
        return result;
    }

    result.success = true;
    result.path = databasePath;
    return result;
}
