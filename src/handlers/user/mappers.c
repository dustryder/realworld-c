#include "user_handlers.h"
#include "main.h"
#include "cJSON.h"

char* create_user_success_response(char *email, char *username, char *token, char *bio, char *image) {
    cJSON *response_body = cJSON_CreateObject();
    cJSON *user_properties = cJSON_CreateObject();

    cJSON_AddItemToObject(user_properties, "email", cJSON_CreateString(email));
    cJSON_AddItemToObject(user_properties, "username", cJSON_CreateString(username));
    cJSON_AddItemToObject(user_properties, "token", cJSON_CreateString(token));
    cJSON_AddItemToObject(user_properties, "bio", bio != NULL ? cJSON_CreateString(bio) : cJSON_CreateNull());
    cJSON_AddItemToObject(user_properties, "image", image != NULL ? cJSON_CreateString(image) : cJSON_CreateNull());

    cJSON_AddItemToObject(response_body, "user", user_properties);

    char *response_string = cJSON_Print(response_body);
    cJSON_Delete(response_body);

    return response_string;
}
