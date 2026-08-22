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
    int created_by;
} ArticleData;

typedef struct {
    ArticleData *data;
    int record_count;
} ArticleDataRecordset;

DataResult update_article_by_slug(PGconn *conn, char *slug, UpdateValue *update_values, size_t update_count);
void delete_article_by_id(PGconn *conn, int id);
DataResult insert_article(PGconn *conn, char* slug, char* title, char* description, char* body, int created_by);
DataResult get_article_data_by_slug(PGconn *conn, char* slug);
DataResult get_all_articles(PGconn *conn, char *author, char *tag, int limit, int offset, char *favorited);
int get_article_count_by_title(PGconn *conn, char *title);
int get_all_articles_count(PGconn *conn);
int get_article_favorite_count(PGconn *conn, char* slug);
int get_user_favorites_article(PGconn *conn, int user_id, char* slug);

void insert_article_favorite(PGconn *conn, int user_id, char* slug);
void delete_article_favorite(PGconn *conn, int user_id, char* slug);
#endif