
#include "users.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <libpq-fe.h>
#include "../lib/db.h"

static UserData *map_user_data(const PGresult *res);

DataResult get_user_data_by_username(PGconn *conn, char* username) {
    FIO_LOG_DEBUG("get_user_data_by_username: username=%s", username);

    char* command = "SELECT *"
                    "FROM \"user\""
                    "WHERE username = $1";
    const char * const data[1] = { username };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_user_data);

    return result;
}

DataResult get_user_data_by_id(PGconn *conn, int id) {
    FIO_LOG_DEBUG("get_user_data_by_id: id: %d", id);
    char str[20];
    sprintf(str, "%d", id);

    char* command = "SELECT *"
                    "FROM \"user\""
                    "WHERE id = $1";
    const char * const data[1] = { str };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_user_data);

    return result;
}

DataResult get_user_by_email(PGconn *conn, char* email) {
    FIO_LOG_DEBUG("get_user_by_email: email: %s", email);

    char* command = "SELECT *"
                    "FROM \"user\""
                    "WHERE email = $1";
    const char * const data[1] = { email };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_user_data);

    return result;
}

DataResult insert_user(PGconn *conn, char* email, char* username, char* password) {
    FIO_LOG_DEBUG("insert_user: email: %s, user: %s, password: %s", email, username, password);

    char* command = "INSERT INTO \"user\" (username, email, password) VALUES"
                    "($1, $2, $3)"
                    "RETURNING *";
    const char * const data[3] = { username, email, password};

    PGresult *data_result = PQexecParams(conn,command,3,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_user_data);
    resolve_user_constraints(&result.error);

    return result;
}

DataResult update_user_data(PGconn *conn, int id, UpdateValue *update_values, size_t value_count) {
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

    PGresult *data_result = PQexecParams(conn, command, value_count + 1, NULL, data, NULL, NULL, 0);

    DataResult result = get_data_result(data_result, map_user_data);

    return result;
}

UserData *map_user_data(const PGresult *res) {

    UserData *data = malloc(sizeof *data);

    data->id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data->username = PQgetvalue(res, 0, 1);
    data->email = PQgetvalue(res, 0, 2);
    data->password = PQgetvalue(res, 0, 3);
    data->bio = PQgetisnull(res, 0, 4) ? NULL : PQgetvalue(res, 0, 4);
    data->image = PQgetisnull(res, 0, 5) ? NULL : PQgetvalue(res, 0, 5);

    return data;
}

void resolve_user_constraints(ErrorValue *value) {
    if (strcmp(value->property, "user_username_key") == 0) {
        value->property = "username";
    }

    if (strcmp(value->property, "user_email_key") == 0) {
        value->property = "email";
    }
}

