#include "users_services.h"

void free_UserServiceResultData(UserServiceResultData *data) {
    free(data->email);
    free(data->token);
    free(data->username);
    free(data->bio);
    free(data->image);
}