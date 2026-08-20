#include "article.h"
#include <libpq-fe.h>
#include "../lib/db.h"

static ArticleData map_article_data(const PGresult *res);
static ArticleDataRecordset *map_many_article_data(const PGresult *res);

DataResult insert_article(char* slug, char* title, char* description, char *body, int created_by) {
    FIO_LOG_DEBUG("insert_article: slug: %s, title: %s, description: %s created_by: %d", slug, title, description, created_by);
    PGconn *connection = get_connection();
    char created_by_str[20];
    sprintf(created_by_str, "%d", created_by);

    char* command = "INSERT INTO \"article\" (slug, title, description, body, created_by) VALUES"
                    "($1, $2, $3, $4, $5)"
                    "RETURNING *";
    const char * const data[5] = { slug, title, description, body, created_by_str};

    PGresult *data_result = PQexecParams(connection,command,5,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_article_data);

    return result;
}

DataResult get_article_data_by_slug(char* slug) {
    FIO_LOG_DEBUG("insert_article: slug: %s", slug);
    PGconn *connection = get_connection();

    char* command = "SELECT * FROM \"article\""
                    "WHERE slug = $1";
    const char * const data[1] = { slug};

    PGresult *data_result = PQexecParams(connection,command,1,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_article_data);

    return result;
}

DataResult get_all_articles(PGconn *conn) {
    FIO_LOG_DEBUG("get_all_articles");
    
    char* command = "SELECT * FROM \"article\"";

    PGresult *data_result = PQexecParams(conn,command,0,NULL,NULL,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_many_article_data);
    ArticleDataRecordset *typ = result.data;

    return result;
}

int get_article_count_by_title(char* title) {
    FIO_LOG_DEBUG("get_article_by_slug: title: %s", title);
    PGconn *connection = get_connection();

    char* command = "SELECT COUNT(*) FROM \"article\""
                    "WHERE title = $1";
    const char * const data[1] = { title, };

    PGresult *data_result = PQexecParams(connection,command,1,NULL,data,NULL,NULL,0);

    int result = get_article_count_result(data_result);

    return result;
}

int get_article_count_result(const PGresult *res) {
    ExecStatusType command_status = PQresultStatus(res);

    if (command_status == PGRES_TUPLES_OK) {
        return strtol(PQgetvalue(res, 0, 0), NULL, 10);
    }

    return 0;
}

ArticleDataRecordset *map_many_article_data(const PGresult *res) {

    int row_count = PQntuples(res);

    ArticleDataRecordset *recordset = malloc(sizeof *recordset);
    recordset->record_count = row_count;
    recordset->data = malloc(row_count * sizeof(*recordset->data));

    for (int i = 0; i < row_count; i++) {
        recordset->data[i].id = strtol(PQgetvalue(res, i, 0), NULL, 10);
        recordset->data[i].slug = PQgetvalue(res, i, 1);
        recordset->data[i].title = PQgetvalue(res, i, 2);
        recordset->data[i].body = PQgetvalue(res, i, 3);
        recordset->data[i].description = PQgetvalue(res, i, 4);
        recordset->data[i].created_at = PQgetvalue(res, i, 5);
        recordset->data[i].updated_at = PQgetvalue(res, i, 6);
        recordset->data[i].created_by = strtol(PQgetvalue(res, i, 7), NULL, 10);
    }

    return recordset;
}

ArticleData map_article_data(const PGresult *res) {
    ArticleData data;

    data.id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data.slug = PQgetvalue(res, 0, 1);
    data.title = PQgetvalue(res, 0, 2);
    data.body = PQgetvalue(res, 0, 3);
    data.description = PQgetvalue(res, 0, 4);
    data.created_at = datetimestamp_to_date(PQgetvalue(res, 0, 5));
    data.updated_at = datetimestamp_to_date(PQgetvalue(res, 0, 6));
    data.created_by = strtol(PQgetvalue(res, 0, 7), NULL, 10);

    return data;
}
