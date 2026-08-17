#include "follow.h"
#include <libpq-fe.h>
#include "../lib/db.h"

FollowData map_follow_data(const PGresult *res) {
    FollowData data;

    data.user_id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data.follow_user_id = strtol(PQgetvalue(res, 0, 1), NULL, 10);

    return data;
}

char *map_follow_constraint(const PGresult *res) {
    char* error = PQresultErrorField(res, PG_DIAG_CONSTRAINT_NAME);

    if (strcmp(error, "user_username_key") == 0) {
        return "username";
    }

    if (strcmp(error, "user_email_key") == 0) {
        return "email";
    }
}

FollowDataResult get_follow_result(const PGresult *res) {
    FollowDataResult result;
    ExecStatusType command_status = PQresultStatus(res);

    if (command_status == PGRES_COMMAND_OK) {
        result.status = DATA_SUCCESS;
    } else if (command_status == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        result.status = DATA_SUCCESS;
        result.data = map_follow_data(res);
    } else if (command_status == PGRES_TUPLES_OK && PQntuples(res) == 0) {
        result.status = DATA_NOT_FOUND;
    } else if (command_status == PGRES_FATAL_ERROR) {
        FIO_LOG_DEBUG("Database request failed: %s\n", PQresultErrorMessage(res));
        result.status = DATA_DUPLICATE;
        ErrorValue error;
        error.error = PQresultErrorMessage(res);

        error.property = map_follow_constraint(res);

        result.error = error;
    } else {
        result.status = DATA_UNKNOWN;
    }

    return result;
}

FollowDataResult insert_follow(int user_id, char* follow_username) {
    FIO_LOG_DEBUG("insert_follow: user_id: %d, follow_username: %s", user_id, follow_username);
    PGconn *connection = get_connection();
    char str[20];
    sprintf(str, "%d", user_id);

    char* command = "INSERT INTO \"follow\" (user_id, user_follow_id) VALUES"
                    "($1, (SELECT id FROM \"user\" WHERE username = $2))"
                    "RETURNING *";
    const char * const data[2] = { str, follow_username};

    PGresult *data_result = PQexecParams(connection,command,2,NULL,data,NULL,NULL,0);

    FollowDataResult result = get_follow_result(data_result);

    return result;
}

FollowDataResult delete_follow(int user_id, char* follow_username) {
    FIO_LOG_DEBUG("delete_follow: user_id: %d, follow_username: %s", user_id, follow_username);
    PGconn *connection = get_connection();
    char str[20];
    sprintf(str, "%d", user_id);

    char* command = "DELETE FROM \"follow\""
                    "WHERE user_id = $1 AND "
                    "user_follow_id=(SELECT id FROM \"user\" WHERE username = $2)";

    const char * const data[2] = { str, follow_username};

    PGresult *data_result = PQexecParams(connection,command,2,NULL,data,NULL,NULL,0);

    FollowDataResult result = get_follow_result(data_result);

    return result;
}