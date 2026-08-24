#include "profiles_services.h"
#include <stdbool.h>
#include "../../data/users.h"

ProfileServiceResultData map_data_to_profile(UserData *data_result, bool following) {
    ProfileServiceResultData data;

    data.username = strdup(data_result->username);
    data.bio = data_result->bio != NULL ? strdup(data_result->bio) : NULL;
    data.image = data_result->image != NULL ? strdup(data_result->image) : NULL;
    data.following = following;

    return data;
}