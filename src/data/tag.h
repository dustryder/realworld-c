#include "../lib/db.h"
#include "../lib/http_helpers.h"

#ifndef TAG_DATA_H
#define TAG_DATA_H
typedef struct {
    int id;
    char* name;
} TagData;

DataResult insert_article_tag(int article_id, char* tag);
char **get_tag_by_article_slug(PGconn *conn, char* slug, int *tag_count);
char **get_tag_names(const PGresult *res, int *tag_count);
#endif