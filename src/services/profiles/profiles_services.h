#include <stdbool.h>
#include "../../data/users.h"

#ifndef PROFILE_SERVICES_H
#define PROFILE_SERVICES_H
typedef struct {
    char* username;
    char* bio;
    char* image;
    bool following;
} ProfileServiceResultData;

typedef enum {
    GET_PROFILE_SUCCESS,
    GET_PROFILE_UNKNOWN
} GetProfileStatus;

typedef struct {
    GetProfileStatus status;
    ProfileServiceResultData result;
} GetProfileByUsernameResult;

GetProfileByUsernameResult get_profile_by_username(char* username);

typedef enum {
    FOLLOW_USER_SUCCESS,
} FollowUserStatus;

typedef struct {
    FollowUserStatus status;
    ProfileServiceResultData result;
} FollowUserResult;

typedef struct {
    FollowUserStatus status;
    ProfileServiceResultData result;
} UnfollowUserResult;

FollowUserResult follow_user(int current_user, char* follow);
UnfollowUserResult unfollow_user(int current_user, char* follow);

ProfileServiceResultData map_data_to_profile(UserData data_result, bool following);
#endif