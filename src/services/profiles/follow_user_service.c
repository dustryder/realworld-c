#include "profiles_services.h"
#include "../../data/follow.h"

FollowUserResult follow_user(PGconn *conn, int current_user, char* follow) {
    FIO_LOG_DEBUG("follow_user: current_user=%d, follow=%s", current_user, follow);

    FollowUserResult result;

    DataResult user_data_result = get_user_data_by_username(conn, follow);

    if (user_data_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        result.error.property = "profile";
        result.error.error = "not found";
    } else {
        DataResult data_result = insert_follow(conn, current_user, follow);
    
        if (data_result.status == DATA_SUCCESS && user_data_result.status == DATA_SUCCESS) {
            result.status = SERVICE_SUCCESS;
            result.result = map_data_to_profile(user_data_result.data, true);
        }
    }

    return result;
}