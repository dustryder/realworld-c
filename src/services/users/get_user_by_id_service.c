#include "users_services.h"
#include "../../data/users.h"
#include "../../lib/token.h"
#include <string.h>

GetUserByIdResult get_user_by_id(int id) {
    GetUserByIdResult result;
    DataResult data_result = get_user_data_by_id(id);

    if (data_result.status == DATA_SUCCESS) {
        result.status = GET_USER_SUCCESS;
        result.result = map_data_to_user(data_result.data);
    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = GET_USER_UNKNOWN;
    }

    return result;
}