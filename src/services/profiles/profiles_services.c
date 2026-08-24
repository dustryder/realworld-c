#include "profiles_services.h"

void free_ProfileServiceResultData(ProfileServiceResultData *data) {
    free(data->bio);
    free(data->image);
    free(data->username);
}