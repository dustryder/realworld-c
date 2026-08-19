#include "../../lib/http_helpers.h"
#include <stdbool.h>
#include "../profiles/profiles_services.h"
#include "../../data/article.h"

#ifndef ARTICLES_SERVICES_H
#define ARTICLES_SERVICES_H

typedef enum {
    CreateArticleSuccess,
} CreateArticleStatus;

typedef struct {
    char* slug;
    char* title;
    char* description;
    char* body;
    char** tagList;
    int tag_count;
    char* createdAt;
    char* updatedAt;
    bool favorited;
    int favoritesCount;
    ProfileServiceResultData author;
} ArticlesServiceResultData;

typedef struct {
    CreateArticleStatus status;
    ArticlesServiceResultData result;
    ErrorValue error;
} CreateArticleResult;

CreateArticleResult create_article(int creator, char* title, char* descrition, char* body, char** tags, int tag_count);

typedef enum {
    GetArticleSuccess,
} GetArticleStatus;

typedef struct {
    GetArticleStatus status;
    ArticlesServiceResultData result;
    ErrorValue error;
} GetArticleResult;

GetArticleResult get_article_by_slug(int user_id, char* slug);

ArticlesServiceResultData map_data_to_article(
    ArticleData *article_data,
    UserData *user_data,
    char** tags,
    int tag_count
);

#endif