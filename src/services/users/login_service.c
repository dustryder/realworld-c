#include "users_services.h"
#include "../../data/users.h"
#include "../../lib/token.h"
#include <string.h>

LoginUserResult login(char* email, char* password) {

    LoginUserResult result;
    UserDataResult data_result = get_user_by_email(email);

    if (data_result.status == DATA_SUCCESS) {

        if (strcmp(password, data_result.data.password) == 0) {
            result.status = LOGIN_USER_SUCCESS;
            result.result = map_data_to_user(data_result.data);
        } else {
            result.status = LOGIN_USER_BAD_PASSWORD;
            result.error.property = "credentials";
            result.error.error = "invalid";
        }

    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = LOGIN_USER_UNKNOWN;
    }

    return result;
}