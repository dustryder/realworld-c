#include "users_services.h"
#include "../../data/users.h"
#include <stdlib.h>
#include "../../lib/token.h"

UserServiceResultData map_data_to_user(UserData data_result) {
    UserServiceResultData data;
    char* jwt = sign_jwt(data_result.id);

    data.username = data_result.username;
    data.email = data_result.email;
    data.token = jwt;
    data.bio = data_result.bio;
    data.image = data_result.image;

    return data;
}