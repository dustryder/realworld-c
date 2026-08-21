#include "tags_services.h"
#include "../../data/tag.h"

GetAllTagsResult get_all_tags(PGconn *conn) {

    GetAllTagsResult result;
    int tag_count;
    char **data_result = get_all_tags_data(conn, &tag_count);

    result.tag_count = tag_count;
    result.result = data_result;

    return result;
}