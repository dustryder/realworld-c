#include "profiles_services.h"
#include "../../data/follow.h"

FollowUserResult follow_user(int current_user, char* follow) {
    FIO_LOG_DEBUG("follow_user: current_user=%d, follow=%s", current_user, follow);

    FollowDataResult data_result = delete_follow(current_user, follow);
    UserDataResult user_data_result = get_user_data_by_username(follow);

    FollowUserResult result;

    if (data_result.status == DATA_SUCCESS && user_data_result.status == DATA_SUCCESS) {
        result.status = FOLLOW_USER_SUCCESS;
        result.result = map_data_to_profile(user_data_result.data, true);
    }

    return result;
}