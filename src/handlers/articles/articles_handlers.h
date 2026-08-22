#include "main.h"
#include "articles_services.h"

#ifndef ARTICLES_HANDLERS_H
#define ARTICLES_HANDLERS_H
typedef struct {
    char* title;
    char* description;
    char* body;
    OptionalArray tags;
} PostArticlePayload;

typedef enum {
    FORMAT_DATETIMESTAMP,
    FORMAT_DATESTAMP,
} DateTimeFormat;

typedef struct {
    char *author;
    char *tag;
    char *favorited;
    int limit;
    int offset;
} GetAllArticleQuery;

typedef struct {
    int limit;
    int offset;
} GetAllFeedQuery;

typedef struct {
    OptionalValue title;
    OptionalValue description;
    OptionalValue body;
    OptionalArray tags;
} PutArticlePayload;

void handle_post_articles(http_s *h);
void handle_get_articles(http_s *h);
void handle_get_all_articles(http_s *h);
void handle_put_articles(http_s *h);
void handle_delete_articles(http_s *h);

void handle_get_all_feed(http_s* h);

void handle_post_favorite(http_s *h);
void handle_delete_favorite(http_s *h);


char *create_article_success_response(ArticlesServiceResultData, bool include_body, DateTimeFormat format);
char *create_many_article_success_response(ArticlesServiceResultData *results, int result_count, int total_count);
#endif