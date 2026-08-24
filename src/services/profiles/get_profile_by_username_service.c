#include "profiles_services.h"
#include "../../data/users.h"
#include "../../lib/mappers.h"

ProfileServiceResult get_profile_by_username(PGconn *conn, char* username) {
    FIO_LOG_DEBUG("get_profile_by_username: username=%s", username);

    DataResult data_result = get_user_data_by_username(conn, username);
    ProfileServiceResult result = {0};

    if (data_result.status == DATA_SUCCESS) {
        result.status = SERVICE_SUCCESS;
        result.result = map_data_to_profile(data_result.data, false);

        free_UserData(data_result.data);
    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        set_error(&result.error, "profile", "not found");
    }

    return result;
}