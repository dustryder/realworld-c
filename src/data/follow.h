#include "../lib/http_helpers.h"
#include "../lib/db.h"

#ifndef FOLLOW_DATA_H
#define FOLLOW_DATA_H
typedef struct {
    int user_id;
    int follow_user_id;
} FollowData;

DataResult insert_follow(int user_id, char* follow_username);
DataResult delete_follow(int user_id, char* follow_username);

#endif