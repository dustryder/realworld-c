#include "../lib/db.h"
#include "../lib/http_helpers.h"

#ifndef TAG_DATA_H
#define TAG_DATA_H
typedef struct {
    int id;
    char* name;
} TagData;

void insert_article_tag(PGconn *conn, int article_id, char* tag);
char **get_tag_by_article_slug(PGconn *conn, char* slug, int *tag_count);
char **get_tag_names(const PGresult *res, int *tag_count);
char **get_all_tags_data(PGconn *conn, int *tag_count);
void delete_article_tags(PGconn *conn, int article_id);
#endif