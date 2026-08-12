#include <libpq-fe.h>
#include "db.h"
#include "dotenv.h"
#include <stdlib.h>

PGconn *get_connection() {
    env_load(".", false);
    char* connectionString = getenv("DATABASE_URL");
    PGconn *connection = PQconnectdb(connectionString);

    ConnStatusType db_status = PQstatus(connection);

    if (db_status != CONNECTION_OK) {
        return NULL;
    }
    
    return connection;
}
