#include "../lib/http_helpers.h"
#include "../lib/db.h"

#ifndef FOLLOW_DATA_H
#define FOLLOW_DATA_H
typedef struct {
    int user_id;
    int follow_user_id;
} FollowData;

DataResult insert_follow(PGconn *conn, int user_id, char* follow_username);
DataResult delete_follow(PGconn *conn, int user_id, char* follow_username);
int get_user_follows_user(PGconn *conn, int user_id, int followed_user_id);

#endif