#include "users_services.h"
#include "../../data/users.h"
#include "../../lib/token.h"
#include <string.h>

void add_field(UpdateValue *values, size_t *count, char *key, char *value) {

    if (value == NULL) {
        return;
    }

    values[*count].key = key;
    values[*count].value = value;

    (*count)++;
}

UpdateUserResult update_user(int id, char* email, char* password, char* username, char* bio, char* image) {

    UpdateValue update_values[5];
    size_t value_count = 0;

    add_field(update_values, &value_count, "email", email);
    add_field(update_values, &value_count, "password", password);
    add_field(update_values, &value_count, "username", username);
    add_field(update_values, &value_count, "bio", bio);
    add_field(update_values, &value_count, "image", image);

    UserDataResult data_result = update_user_data(id, update_values, value_count);

    UpdateUserResult result;

    if (data_result.status == DATA_SUCCESS) {
        result.status = GET_USER_SUCCESS;
        result.result = map_data_to_user(data_result.data);
    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = GET_USER_UNKNOWN;
    }

    return result;
}