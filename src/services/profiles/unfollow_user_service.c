#include "profiles_services.h"
#include "../../data/follow.h"
#include "../../lib/mappers.h"

ProfileServiceResult unfollow_user(PGconn *conn, int current_user, char* follow) {
    FIO_LOG_DEBUG("unfollow_user: current_user=%d, follow=%s", current_user, follow);

    ProfileServiceResult result = {0};

    DataResult user_data_result = get_user_data_by_username(conn, follow);

    if (user_data_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        set_error(&result.error, "profile", "not found");
    } else {
        DataResult data_result = delete_follow(conn, current_user, follow);

        if (data_result.status == DATA_SUCCESS && user_data_result.status == DATA_SUCCESS) {
            result.status = SERVICE_SUCCESS;
            result.result = map_data_to_profile(user_data_result.data, false);
        }

        free_UserData(user_data_result.data);
    }

    return result;
}