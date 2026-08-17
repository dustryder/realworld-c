#include "profile_handlers.h"
#include "../../services/profiles/profiles_services.h"
#include "cJSON.h"

char *create_success_profile_response(ProfileServiceResultData data) {
    cJSON *response_body = cJSON_CreateObject();
    cJSON *profile = cJSON_CreateObject();

    cJSON_AddItemToObject(profile, "username", cJSON_CreateString(data.username));
    cJSON_AddItemToObject(profile, "bio", data.bio != NULL ? cJSON_CreateString(data.bio) : cJSON_CreateNull());
    cJSON_AddItemToObject(profile, "image", data.image != NULL ? cJSON_CreateString(data.image) : cJSON_CreateNull());

    cJSON_AddItemToObject(profile, "following", cJSON_CreateBool(data.following));

    cJSON_AddItemToObject(response_body, "profile", profile);

    char *response_string = cJSON_Print(response_body);
    cJSON_Delete(response_body);

    return response_string;    
}