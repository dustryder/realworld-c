#include "profiles_services.h"
#include <stdbool.h>
#include "../../data/users.h"

ProfileServiceResultData map_data_to_profile(UserData data_result, bool following) {
    ProfileServiceResultData data;

    data.username = data_result.username;
    data.bio = data_result.bio;
    data.image = data_result.image;
    data.following = following;

    return data;
}