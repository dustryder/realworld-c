#include "profiles_services.h"
#include "../../data/follow.h"
#include "../../lib/mappers.h"

ProfileServiceResult follow_user(PGconn *conn, int current_user, char* follow) {
    FIO_LOG_DEBUG("follow_user: current_user=%d, follow=%s", current_user, follow);

    ProfileServiceResult result = {0};

    DataResult user_data_result = get_user_data_by_username(conn, follow);
    UserData *user_data = user_data_result.data;

    if (user_data_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        set_error(&result.error, "profile", "not found");
    } else {
        DataResult data_result = insert_follow(conn, current_user, follow);
    
        if (data_result.status == DATA_SUCCESS && user_data_result.status == DATA_SUCCESS) {
            result.status = SERVICE_SUCCESS;
            result.result = map_data_to_profile(user_data, true);
        } else if (data_result.status == DATA_DUPLICATE) {
            result.status = SERVICE_DUPLICATE;
            set_error(&result.error, "profile", "already following");
        }

        free_UserData(user_data);
    }

    return result;
}