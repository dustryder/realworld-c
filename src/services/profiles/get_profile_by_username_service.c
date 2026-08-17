#include "profiles_services.h"
#include "../../data/users.h"

GetProfileByUsernameResult get_profile_by_username(char* username) {
    FIO_LOG_DEBUG("get_profile_by_username: username=%s", username);

    UserDataResult data_result = get_user_data_by_username(username);
    GetProfileByUsernameResult result;

    if (data_result.status == DATA_SUCCESS) {
        result.status = GET_PROFILE_SUCCESS;
        result.result = map_data_to_profile(data_result.data, false);
    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = GET_PROFILE_UNKNOWN;
        result.error.property = "profile";
        result.error.error = "not found";
    }

    return result;
}