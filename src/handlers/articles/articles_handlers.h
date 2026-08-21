#include "main.h"
#include "articles_services.h"

#ifndef ARTICLES_HANDLERS_H
#define ARTICLES_HANDLERS_H
typedef struct {
    char* title;
    char* description;
    char* body;
    char** tags;
    int tag_count;
} PostArticlePayload;

typedef enum {
    FORMAT_DATETIMESTAMP,
    FORMAT_DATESTAMP,
} DateTimeFormat;

typedef struct {
    char *author;
    char *tag;
} GetAllArticleQuery;

void handle_post_articles(http_s* h);
void handle_get_articles(http_s* h);
void handle_get_all_articles(http_s* h);

char *create_article_success_response(ArticlesServiceResultData, bool include_body, DateTimeFormat format);
char *create_many_article_success_response(ArticlesServiceResultData *results, int result_count);
#endif