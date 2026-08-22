#include "follow.h"
#include "users.h"
#include <libpq-fe.h>
#include "../lib/db.h"

static FollowData map_follow_data(const PGresult *res);

DataResult insert_follow(PGconn *conn, int user_id, char* follow_username) {
    FIO_LOG_DEBUG("insert_follow: user_id: %d, follow_username: %s", user_id, follow_username);

    char str[20];
    sprintf(str, "%d", user_id);

    char* command = "INSERT INTO \"follow\" (user_id, user_follow_id) VALUES "
                    "($1, (SELECT id FROM \"user\" WHERE username = $2)) "
                    "RETURNING *";
    const char * const data[2] = { str, follow_username};

    PGresult *data_result = PQexecParams(conn,command,2,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_follow_data);

    return result;
}

DataResult delete_follow(PGconn *conn, int user_id, char* follow_username) {
    FIO_LOG_DEBUG("delete_follow: user_id: %d, follow_username: %s", user_id, follow_username);
    PGconn *connection = get_connection();
    char str[20];
    sprintf(str, "%d", user_id);

    char *command = "DELETE FROM \"follow\""
                    "WHERE user_id = $1 AND "
                    "user_follow_id=(SELECT id FROM \"user\" WHERE username = $2)";

    const char * const data[2] = { str, follow_username};

    PGresult *data_result = PQexecParams(conn,command,2,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_follow_data);
    resolve_user_constraints(&result.error);

    return result;
}

int get_user_follows_user(PGconn *conn, int user_id, int followed_user_id) {
    FIO_LOG_DEBUG("get_user_follows_user: user_id: %d, followed_user_id: %d", user_id, followed_user_id);
    
    char user_id_str[20];
    sprintf(user_id_str, "%d", user_id);
    char followed_user_id_str[20];
    sprintf(followed_user_id_str, "%d", followed_user_id);

    char *command = "SELECT 1 FROM follow WHERE user_id = $1 AND user_follow_id = $2";

    const char * const data[2] = { user_id_str, followed_user_id_str };

    PGresult *data_result = PQexecParams(conn,command,2,NULL,data,NULL,NULL,0);

    return get_integer_result(data_result);
}

FollowData map_follow_data(const PGresult *res) {
    FollowData data;

    data.user_id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data.follow_user_id = strtol(PQgetvalue(res, 0, 1), NULL, 10);

    return data;
}

int get_integer_result(const PGresult *res) {
    ExecStatusType command_status = PQresultStatus(res);

    if (command_status == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        return strtol(PQgetvalue(res, 0, 0), NULL, 10);
    } else if (command_status == PGRES_TUPLES_OK && PQntuples(res) == 0) {
        return 0;
    } else if (command_status == PGRES_FATAL_ERROR) {
        FIO_LOG_DEBUG("Database request failed: %s\n", PQresultErrorMessage(res));
    }

    return 0;
}