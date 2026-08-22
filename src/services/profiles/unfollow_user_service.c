#include "profiles_services.h"
#include "../../data/follow.h"

UnfollowUserResult unfollow_user(PGconn *conn, int current_user, char* follow) {
    FIO_LOG_DEBUG("unfollow_user: current_user=%d, follow=%s", current_user, follow);

    UnfollowUserResult result;

    DataResult user_data_result = get_user_data_by_username(conn, follow);

    if (user_data_result.status == DATA_NOT_FOUND) {
        result.status = FOLLOW_USER_UNKNOWN;
        result.error.property = "profile";
        result.error.error = "not found";
    } else {
        DataResult data_result = delete_follow(conn, current_user, follow);

        if (data_result.status == DATA_SUCCESS && user_data_result.status == DATA_SUCCESS) {
            result.status = FOLLOW_USER_SUCCESS;
            result.result = map_data_to_profile(user_data_result.data, false);
        }
    }

    return result;
}