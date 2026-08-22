#include "comments_handlers.h"
#include "../../services/comments/comments_services.h"
#include "cJSON.h"
#include "../../lib/string_helpers.h"

static cJSON *create_comment_json(CommentsServiceResultData result);

char *create_many_comment_success_response(CommentsServiceResultData *result, int result_count) {
    cJSON *response_body = cJSON_CreateObject();

    cJSON *comment_array = cJSON_CreateArray();

    for (int i = 0; i < result_count; i++) {
        cJSON *comment_body = create_comment_json(result[i]);
        cJSON_AddItemToArray(comment_array, comment_body);
    }

    cJSON_AddItemToObject(response_body, "comments", comment_array);

    char *response_string = cJSON_Print(response_body);
    cJSON_Delete(response_body);

    return response_string;
}

char *create_comment_success_response(CommentsServiceResultData result) {
    cJSON *response_body = cJSON_CreateObject();
    
    cJSON *comment_body = create_comment_json(result);

    cJSON_AddItemToObject(response_body, "comment", comment_body);

    char *response_string = cJSON_Print(response_body);
    cJSON_Delete(response_body);

    return response_string;
}

cJSON *create_comment_json(CommentsServiceResultData result) {
    cJSON *comment_body = cJSON_CreateObject();
    cJSON *author_body = cJSON_CreateObject();

    cJSON_AddItemToObject(author_body, "username", cJSON_CreateString(result.author.username));
    cJSON_AddItemToObject(author_body, "bio", cJSON_CreateString(result.author.bio));
    cJSON_AddItemToObject(author_body, "image", cJSON_CreateString(result.author.image));
    cJSON_AddItemToObject(author_body, "following", cJSON_CreateBool(result.author.following));

    cJSON_AddItemToObject(comment_body, "id", cJSON_CreateNumber(result.id));
    cJSON_AddItemToObject(comment_body, "createdAt", cJSON_CreateString(datetimestamp_to_datetimestamp(result.created_at)));
    cJSON_AddItemToObject(comment_body, "updatedAt", cJSON_CreateString(datetimestamp_to_datetimestamp(result.updated_at)));
    cJSON_AddItemToObject(comment_body, "body", cJSON_CreateString(result.body));

    cJSON_AddItemToObject(comment_body, "author", author_body);

    return comment_body;
}