#include <libpq-fe.h>

#ifndef TAGS_SERVICES_H
#define TAGS_SERVICES_H

typedef struct {
    char **result;
    int tag_count;
} GetAllTagsResult;

GetAllTagsResult get_all_tags(PGconn *conn);
#endif