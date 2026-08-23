#include <libpq-fe.h>
#include "db.h"
#include "dotenv.h"
#include <stdlib.h>
#include "http_helpers.h"
#include "main.h"

PGconn *get_connection() {
    env_load(".", false);
    char* connectionString = getenv("DATABASE_URL");
    PGconn *connection = PQconnectdb(connectionString);

    ConnStatusType db_status = PQstatus(connection);

    if (db_status != CONNECTION_OK) {
        PQfinish(connection);
        FIO_LOG_ERROR("Could not establish database connection with code: %d\n", db_status);
        FIO_LOG_ERROR("Database error: %s\n", PQerrorMessage(connection));
        
        return NULL;
    }
    
    return connection;
}

DataResult get_data_result(const PGresult *res, Mapper mapper) {
    DataResult result;
    ExecStatusType command_status = PQresultStatus(res);

    if (command_status == PGRES_COMMAND_OK) {
        result.status = DATA_SUCCESS;
    } else if (command_status == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        result.status = DATA_SUCCESS;
        result.data = mapper(res);
    } else if (command_status == PGRES_TUPLES_OK && PQntuples(res) == 0) {
        result.status = DATA_NOT_FOUND;
    } else if (command_status == PGRES_FATAL_ERROR) {
        FIO_LOG_DEBUG("Database request failed: %s\n", PQresultErrorMessage(res));
        result.status = DATA_DUPLICATE;
        ErrorValue error;
        error.message = PQresultErrorMessage(res);
        error.property = PQresultErrorField(res, PG_DIAG_CONSTRAINT_NAME);
        result.error = error;
    } else {
        FIO_LOG_DEBUG("Database request failed: %d\n", command_status);
        result.status = DATA_UNKNOWN;
    }

    return result;
}