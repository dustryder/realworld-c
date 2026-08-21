#include "tags_handlers.h"
#include "../../services/tags/tags_services.h"
#include "../../lib/constants.h"
#include "cJSON.h"

char *create_get_tags_success_response(GetAllTagsResult result);

void handle_get_tags(http_s* h) {
    FIO_LOG_DEBUG("handle_get_tags");

    char* response_body;
    GetAllTagsResult result = get_all_tags(h->udata);

    h->status = HTTP_SUCCESS;
    response_body = create_get_tags_success_response(result);

    http_send_body(h, response_body, strlen(response_body));
}

char *create_get_tags_success_response(GetAllTagsResult result) {
    cJSON *response_body = cJSON_CreateObject();
    cJSON *tags_body = cJSON_CreateArray();
    
    for (int i = 0; i < result.tag_count; i++) {
        cJSON_AddItemToArray(tags_body, cJSON_CreateString(result.result[i]));
    }

    cJSON_AddItemToObject(response_body, "tags", tags_body);

    char *response_string = cJSON_Print(response_body);
    cJSON_Delete(response_body);

    return response_string;
}