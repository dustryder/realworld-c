#include "comments_services.h"

void free_CommentsServiceResultData(CommentsServiceResultData *data) {
    free(data->author.bio);
    free(data->author.image);
    free(data->author.username);
    free(data->body);
    free(data->created_at);
    free(data->updated_at);
}