#include "data.h"
#include "../../lib/http_helpers.h"

#ifndef FOLLOW_DATA_H
#define FOLLOW_DATA_H
typedef struct {
    int user_id;
    int follow_user_id;
} FollowData;

typedef struct {
    DataStatus status;
    FollowData data;
    ErrorValue error;
} FollowDataResult;

FollowDataResult insert_follow(int user_id, char* follow_username);
FollowDataResult delete_follow(int user_id, char* follow_username);

#endif