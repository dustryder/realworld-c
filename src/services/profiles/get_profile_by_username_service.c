#include "profiles_services.h"
#include "../../data/users.h"
#include "../../lib/mappers.h"
#include "../../data/follow.h"

ProfileServiceResult get_profile_by_username(PGconn *conn, char* username, int user_id) {
    FIO_LOG_DEBUG("get_profile_by_username: username=%s", username);

    DataResult data_result = get_user_data_by_username(conn, username);
    UserData *user_data = data_result.data;
    ProfileServiceResult result = {0};

    if (data_result.status == DATA_SUCCESS) {
        result.status = SERVICE_SUCCESS;
        int user_follows_user = user_id != -1 ? get_user_follows_user(conn, user_id, user_data->id) : 0;
        result.result = map_data_to_profile(data_result.data, user_follows_user);

        free_UserData(data_result.data);
    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        set_error(&result.error, "profile", "not found");
    }

    return result;
}