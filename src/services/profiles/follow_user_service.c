#include "profiles_services.h"
#include "../../data/follow.h"

FollowUserResult follow_user(int current_user, char* follow) {
    FIO_LOG_DEBUG("follow_user: current_user=%d, follow=%s", current_user, follow);

    FollowUserResult result;

    DataResult user_data_result = get_user_data_by_username(follow);

    if (user_data_result.status == DATA_NOT_FOUND) {
        result.status = FOLLOW_USER_UNKNOWN;
        result.error.property = "profile";
        result.error.error = "not found";
    } else {
        DataResult data_result = insert_follow(current_user, follow);
    
        if (data_result.status == DATA_SUCCESS && user_data_result.status == DATA_SUCCESS) {
            result.status = FOLLOW_USER_SUCCESS;
            result.result = map_data_to_profile(user_data_result.data, true);
        }
    }

    return result;
}