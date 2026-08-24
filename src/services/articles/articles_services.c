#include "articles_services.h"

void free_ArticlesServiceResultData(ArticlesServiceResultData *data) {
    
    free(data->body);
    free(data->createdAt);
    free(data->description);
    free(data->slug);
    free(data->title);
    free(data->updatedAt);
    free(data->author.bio);
    free(data->author.image);
    free(data->author.username);

    for (int i = 0; i < data->tag_count; i++) {
        free(data->tagList[i]);
    }

    free(data->tagList);
}