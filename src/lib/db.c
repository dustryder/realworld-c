#include <libpq-fe.h>
#include "db.h"

PGconn *get_connection() {
    char* connectionString = "postgresql://postgres:Gryphon1!@localhost:5432/realworld";
    PGconn *connection = PQconnectdb(connectionString);

    ConnStatusType db_status = PQstatus(connection);

    if (db_status != CONNECTION_OK) {
        return NULL;
    }
    
    return connection;
}
