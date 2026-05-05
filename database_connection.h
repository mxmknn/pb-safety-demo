#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <QString>

struct DatabaseInitResult {
    bool success = false;
    QString path;
    QString errorMessage;
};

DatabaseInitResult initializeDatabaseConnection();

#endif
