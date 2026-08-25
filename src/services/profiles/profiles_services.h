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

typedef struct {
    ServiceStatus status;
    ProfileServiceResultData result;
    ErrorValue error;
} ProfileServiceResult;

ProfileServiceResult follow_user(PGconn *conn, int current_user, char* follow);
ProfileServiceResult unfollow_user(PGconn *conn, int current_user, char* follow);
ProfileServiceResult get_profile_by_username(PGconn *conn, char* username, int user_id);

ProfileServiceResultData map_data_to_profile(UserData *data_result, bool following);
void free_ProfileServiceResultData(ProfileServiceResultData *data);
#endif