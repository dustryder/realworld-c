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

char* create_post_user_failure() {

  cJSON *arr = cJSON_CreateArray();
  cJSON *str = cJSON_CreateString("Oh no");
  cJSON_AddItemToArray(arr, str);

  cJSON *error = cJSON_CreateObject();
  cJSON_AddItemToObject(error, "username", arr);

  cJSON *response_body = cJSON_CreateObject();
  cJSON_AddItemToObject(response_body, "errors", error);

  char *response_string = cJSON_Print(response_body);
  cJSON_Delete(response_body);

  return response_string;
}

char *create_post_user_failure_from_errors(ErrorValue* errors, size_t error_count) {

  cJSON *response_body = cJSON_CreateObject();
  cJSON *error_body = cJSON_CreateObject();

  for (int i = 0; i < error_count; i++) {
    ErrorValue currentError = errors[i];

    if (cJSON_GetObjectItem(error_body, currentError.property) != NULL) {
      cJSON *errorArray = cJSON_GetObjectItem(error_body, currentError.property);
      cJSON *errorArrayItem = cJSON_CreateString(currentError.error);
      cJSON_AddItemToArray(errorArray, errorArrayItem);
    } else {
      cJSON *errorArray = cJSON_CreateArray();
      cJSON *errorArrayItem = cJSON_CreateString(currentError.error);
      cJSON_AddItemToArray(errorArray, errorArrayItem);
      cJSON_AddItemToObject(error_body, currentError.property, errorArray);
    }
  }

  cJSON_AddItemToObject(response_body, "errors", error_body);

  char *response_string = cJSON_Print(response_body);
  cJSON_Delete(response_body);

  return response_string;
}