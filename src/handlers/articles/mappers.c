#include "articles_services.h"
#include "articles_handlers.h"
#include "cJSON.h"

char* create_article_success_response(ArticlesServiceResultData result) {
    cJSON *response_body = cJSON_CreateObject();
    cJSON *article_properties = cJSON_CreateObject();
    cJSON *author_properties = cJSON_CreateObject();

    cJSON_AddItemToObject(author_properties, "username", cJSON_CreateString(result.author.username));
    cJSON_AddItemToObject(author_properties, "bio", cJSON_CreateString(result.author.bio));
    cJSON_AddItemToObject(author_properties, "image", cJSON_CreateString(result.author.image));
    cJSON_AddItemToObject(author_properties, "following", cJSON_CreateBool(result.author.following));

    cJSON_AddItemToObject(article_properties, "slug", cJSON_CreateString(result.slug));
    cJSON_AddItemToObject(article_properties, "title", cJSON_CreateString(result.title));
    cJSON_AddItemToObject(article_properties, "description", cJSON_CreateString(result.description));
    cJSON_AddItemToObject(article_properties, "body", cJSON_CreateString(result.body));
    cJSON_AddItemToObject(article_properties, "tagList", cJSON_CreateStringArray((const char *const *)result.tagList, result.tag_count));
    cJSON_AddItemToObject(article_properties, "createdAt", cJSON_CreateString(result.createdAt));
    cJSON_AddItemToObject(article_properties, "updatedAt", cJSON_CreateString(result.updatedAt));
    cJSON_AddItemToObject(article_properties, "favorited", cJSON_CreateBool(result.favorited));
    cJSON_AddItemToObject(article_properties, "favoritesCount", cJSON_CreateNumber(result.favoritesCount));

    cJSON_AddItemToObject(article_properties, "author", author_properties);
    cJSON_AddItemToObject(response_body, "article", article_properties);

    char *response_string = cJSON_Print(response_body);
    cJSON_Delete(response_body);

    return response_string;
}