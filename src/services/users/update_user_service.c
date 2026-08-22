#include "users_services.h"
#include "../../data/users.h"
#include "../../lib/token.h"
#include <string.h>
#include "../../lib/type.h"

UserServiceResult update_user(
    PGconn *conn,
    int id, 
    OptionalValue email, 
    OptionalValue password, 
    OptionalValue username, 
    OptionalValue bio, 
    OptionalValue image
) {

    UpdateValue update_values[5];
    size_t value_count = 0;

    add_field(update_values, &value_count, "email", email);
    add_field(update_values, &value_count, "password", password);
    add_field(update_values, &value_count, "username", username);
    add_field(update_values, &value_count, "bio", bio);
    add_field(update_values, &value_count, "image", image);

    DataResult data_result = update_user_data(conn, id, update_values, value_count);
    UserData *user_data = data_result.data;

    UserServiceResult result;

    if (data_result.status == DATA_SUCCESS) {
        result.status = SERVICE_SUCCESS;
        result.data = map_data_to_user(user_data);
    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_UNKNOWN;
    }

    return result;
}