#include "users_services.h"
#include "../../data/users.h"
#include <stdlib.h>
#include "../../lib/token.h"

UserServiceResultData map_data_to_user(UserData *data_result) {
    UserServiceResultData data;
    char* jwt = sign_jwt(data_result->id);

    data.username = strdup(data_result->username);
    data.email = strdup(data_result->email);
    data.token = jwt;
    data.bio = data_result->bio != NULL ? strdup(data_result->bio) : NULL;
    data.image = data_result->image != NULL ? strdup(data_result->image) : NULL;

    return data;
}