
#include "users.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <libpq-fe.h>
#include "../lib/db.h"

UserData map_user_data(const PGresult *res) {
    UserData data;

    data.id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data.username = PQgetvalue(res, 0, 1);
    data.email = PQgetvalue(res, 0, 2);
    data.password = PQgetvalue(res, 0, 3);
    data.bio = PQgetisnull(res, 0, 4) ? NULL : PQgetvalue(res, 0, 4);
    data.image = PQgetisnull(res, 0, 5) ? NULL : PQgetvalue(res, 0, 5);

    return data;
}

UserDataResult get_result(const PGresult *res) {
    UserDataResult result;
    ExecStatusType command_status = PQresultStatus(res);

    if (command_status == PGRES_COMMAND_OK) {
        result.status = DATA_SUCCESS;
    } else if (command_status == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        result.status = DATA_SUCCESS;
        result.data = map_user_data(res);
    } else if (command_status == PGRES_TUPLES_OK && PQntuples(res) == 0) {
        result.status = DATA_NOT_FOUND;
    } else if (command_status == PGRES_FATAL_ERROR) {
        printf("%s\n", PQerrorMessage(res));
        result.status = DATA_DUPLICATE;
    } else {
        result.status = DATA_UNKNOWN;
    }

    return result;
}

UserDataResult get_user_data_by_id(int id) {

    PGconn *connection = get_connection();
    char str[20];
    sprintf(str, "%d", id);

    char* command = "SELECT *"
                    "FROM \"user\""
                    "WHERE id = $1";
    const char * const data[1] = { str };

    PGresult *data_result = PQexecParams(connection,command,1,NULL,data,NULL,NULL,0);

    UserDataResult result = get_result(data_result);

    return result;
}

UserDataResult get_user_by_email(char* email) {

    PGconn *connection = get_connection();

    char* command = "SELECT *"
                    "FROM \"user\""
                    "WHERE email = $1";
    const char * const data[1] = { email };

    PGresult *data_result = PQexecParams(connection,command,1,NULL,data,NULL,NULL,0);

    UserDataResult result = get_result(data_result);

    return result;
}

UserDataResult insert_user(char* email, char* username, char* password) {

    PGconn *connection = get_connection();

    char* command = "INSERT INTO \"user\" (username, email, password) VALUES"
                    "($1, $2, $3)"
                    "RETURNING *";
    const char * const data[3] = { username, email, password};

    PGresult *data_result = PQexecParams(connection,command,3,NULL,data,NULL,NULL,0);

    UserDataResult result = get_result(data_result);

    return result;
}

UserDataResult update_user_data(int id, UpdateValue *update_values, size_t value_count) {

    PGconn *connection = get_connection();
    char string_id[20];
    sprintf(string_id, "%d", id);

    char *data[value_count + 1];
    char update_substring[20 * (value_count + 1)];
    memset(update_substring, 0, sizeof(update_substring));

    char *base_command = "UPDATE \"user\" "
                         "SET %s"
                         " WHERE id = $1 "
                         "RETURNING *";

    data[0] = string_id;

    for (int i = 1; i < value_count + 1; i++) {
        data[i] = update_values[i - 1].value;
    }

    for (int i = 0; i < value_count; i++) {
        sprintf(update_substring + strlen(update_substring), "%s = $%d", update_values[i].key, i + 2);

        if (i != value_count - 1) {
            sprintf(update_substring + strlen(update_substring), ",");
        }
    };

    char command[strlen(base_command) + strlen(update_substring)];
    sprintf(command, base_command, update_substring);

    PGresult *data_result = PQexecParams(connection, command, value_count + 1, NULL, data, NULL, NULL, 0);

    UserDataResult result = get_result(data_result);

    return result;
}