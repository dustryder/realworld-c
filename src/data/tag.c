#include "tag.h"
#include <libpq-fe.h>
#include "../lib/db.h"

static TagData map_tag_data(const PGresult *res);

void insert_tag(char* tag) {
    FIO_LOG_DEBUG("insert_tag: tag: %s", tag);
    PGconn *connection = get_connection();

    char *command = "INSERT INTO \"tag\"(name) VALUES ($1) ON CONFLICT (name) DO NOTHING;";

    const char * const data[1] = { tag };

    PGresult *data_result = PQexecParams(connection,command,1,NULL,data,NULL,NULL,0);

    ExecStatusType command_status = PQresultStatus(data_result);

    return;
}

void delete_article_tags(PGconn *conn, int article_id) {
    FIO_LOG_DEBUG("delete_article_tag: article_id=%d", article_id);
    char article_id_str[20];
    sprintf(article_id_str, "%d", article_id);

    char *command = "DELETE FROM article_tag WHERE article_id = $1";

    const char * const data[1] = { article_id_str };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    return;
}

char **get_tag_by_article_slug(PGconn *conn, char* slug, int *tag_count) {
    FIO_LOG_DEBUG("get_tag_by_article_slug: slug: %s", slug);

    char *command = "SELECT tag.name "
                    "FROM tag "
                    "JOIN article_tag ON article_tag.tag_id = tag.id "
                    "JOIN article ON article_tag.article_id = article.id "
                    "WHERE article.slug = $1";

    const char * const data[1] = { slug };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    char **tag_names = get_tag_names(data_result, tag_count);

    return tag_names;
}

char **get_all_tags_data(PGconn *conn, int *tag_count) {
    FIO_LOG_DEBUG("get_all_tags");

    char *command = "SELECT tag.name "
                    "FROM tag";

    PGresult *data_result = PQexecParams(conn,command,0,NULL,NULL,NULL,NULL,0);

    char **tag_names = get_tag_names(data_result, tag_count);

    return tag_names;
}

DataResult insert_article_tag(int article_id, char* tag) {
    FIO_LOG_DEBUG("insert_article_tag: article_id: %d, tag: %s", article_id, tag);
    PGconn *connection = get_connection();
    char article_id_str[20];
    sprintf(article_id_str, "%d", article_id);

    char *command = "INSERT INTO \"article_tag\" (article_id, tag_id) "
                                "SELECT $2, id "
                                "FROM tag "
                                "WHERE name = $1 "
                                "ON CONFLICT DO NOTHING"; 
    const char * const data[2] = { tag, article_id_str };

    insert_tag(tag);

    PGresult *data_result = PQexecParams(connection,command,2,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_tag_data);

    return result;
}

char **get_tag_names(const PGresult *res, int *tag_count) {
    ExecStatusType command_status = PQresultStatus(res);

    if (command_status == PGRES_TUPLES_OK) {
        int row_count = PQntuples(res);
        *tag_count = row_count;
        char **tags = malloc(sizeof(char *) * row_count);

        for (int i = 0; i < row_count; i++) {
            tags[i] = strdup(PQgetvalue(res, i, 0));
        }

        return tags;
    }
}

TagData map_tag_data(const PGresult *res) {
    TagData data;

    data.id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data.name = PQgetvalue(res, 0, 1);

    return data;
}
