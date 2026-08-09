
#include "users.h"
#include <string.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include "../lib/db.h"

UserData map_user_data(const PGresult *res) {
    UserData data;

    data.id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data.username = PQgetvalue(res, 0, 1);
    data.email = PQgetvalue(res, 0, 2);

    return data;
}

UserDataResult insertUser(char* email, char* username, char* password) {

    PGconn *connection = get_connection();
    UserDataResult result;
    char* command = "INSERT INTO \"user\" (username, email, password) VALUES"
                    "($1, $2, $3)"
                    "RETURNING id, username, email";
                  
    const char * const data[3] = { username, email, password};

    PGresult *data_result = PQexecParams(connection,command,3,NULL,data,NULL,NULL,0);

    ExecStatusType command_status = PQresultStatus(data_result);

    if (command_status == PGRES_COMMAND_OK) {
        result.status = DataStatusSuccess;
    } else if (command_status == PGRES_TUPLES_OK) {
        result.status = DataStatusSuccess;
        result.data = map_user_data(data_result);
    } else if (command_status == PGRES_FATAL_ERROR) {
        result.status = DataStatusDuplicate;
    } else {
        result.status = DataStatusUnknown;
    }

    return result;
}