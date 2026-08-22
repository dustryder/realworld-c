#include "follow.h"
#include "users.h"
#include <libpq-fe.h>
#include "../lib/db.h"

static FollowData map_follow_data(const PGresult *res);

DataResult insert_follow(PGconn *conn, int user_id, char* follow_username) {
    FIO_LOG_DEBUG("insert_follow: user_id: %d, follow_username: %s", user_id, follow_username);

    char str[20];
    sprintf(str, "%d", user_id);

    char* command = "INSERT INTO \"follow\" (user_id, user_follow_id) VALUES"
                    "($1, (SELECT id FROM \"user\" WHERE username = $2))"
                    "RETURNING *";
    const char * const data[2] = { str, follow_username};

    PGresult *data_result = PQexecParams(conn,command,2,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_follow_data);
    resolve_user_constraints(&result.error);

    return result;
}

DataResult delete_follow(PGconn *conn, int user_id, char* follow_username) {
    FIO_LOG_DEBUG("delete_follow: user_id: %d, follow_username: %s", user_id, follow_username);
    PGconn *connection = get_connection();
    char str[20];
    sprintf(str, "%d", user_id);

    char* command = "DELETE FROM \"follow\""
                    "WHERE user_id = $1 AND "
                    "user_follow_id=(SELECT id FROM \"user\" WHERE username = $2)";

    const char * const data[2] = { str, follow_username};

    PGresult *data_result = PQexecParams(conn,command,2,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_follow_data);
    resolve_user_constraints(&result.error);

    return result;
}

FollowData map_follow_data(const PGresult *res) {
    FollowData data;

    data.user_id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data.follow_user_id = strtol(PQgetvalue(res, 0, 1), NULL, 10);

    return data;
}