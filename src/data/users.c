
#include "users.h"
#include <libpq-fe.h>
#include "../lib/db.h"

void insertUser(char* email, char* username, char* password) {

    PGconn *connection = get_connection();
    char* command = "INSERT INTO \"user\" (username, email, password) VALUES"
                    "($1, $2, $3)";
                  
    const char * const data[3] = { username, email, password};

    PGresult *result = PQexecParams(connection,command,3,NULL,data,NULL,NULL,0);

    ExecStatusType command_status = PQresultStatus(result);

    if (command_status != PGRES_COMMAND_OK) {
        printf("Failure: %d\n", command_status);
        return;
    }

    printf("Success: %s\n");
}