#include "../lib/db.h"
#include "../lib/http_helpers.h"

#ifndef ARTICLE_DATA_H
#define ARTICLE_DATA_H
typedef struct {
    int id;
    char* slug;
    char* title;
    char* description;
    char* body;
    char* created_at;
    char* updated_at;
    int* created_by;
} ArticleData;

typedef struct {
    ArticleData *data;
    int record_count;
} ArticleDataRecordset;

DataResult insert_article(char* slug, char* title, char* description, char* body, int created_by);
DataResult get_article_data_by_slug(char* slug);
DataResult get_all_articles(PGconn *conn, char* author);
int get_article_count_by_title(char* title);
#endif