#include "main.h"
#include "articles_handlers.h"
#include "../../lib/http_helpers.h"
#include "../../lib/constants.h"
#include "../../lib/type.h"

static PutArticlePayload parse_put_article_body(FIOBJ *raw_body);

void handle_put_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_put_article");

    int id = parse_request_user(h->params);
    char* response_body = "";

    PutArticlePayload payload = parse_put_article_body(h->body);
    char *slug = parse_path_param(h->params, "slug");

    if (payload.tags.is_present && payload.tags.value == NULL) {
      h->status = HTTP_UNPROCESSABLE_ENTITY;
      http_send_body(h, "", 0);
      return;
    }

    UpdateArticleResult result = update_article(
      h->udata,
      slug,
      payload.title,
      payload.description,
      payload.body,
      payload.tags,
      id
    );

    if (result.status == SERVICE_SUCCESS) {
      response_body = create_article_success_response(result.result, true, FORMAT_DATESTAMP);
      h->status = HTTP_SUCCESS;
    } else if (result.status == SERVICE_NOT_FOUND) {
      response_body = create_failure_body_from_error(result.error);
      h->status = HTTP_NOT_FOUND;
    } else if (result.status == SERVICE_UNAUTHORIZED) {
      response_body = create_failure_body_from_error(result.error);
      h->status = HTTP_FORBIDDEN;
    }

    if (payload.tags.value != NULL) free(payload.tags.value);

    http_send_body(h, response_body, strlen(response_body));
}

PutArticlePayload parse_put_article_body(FIOBJ *raw_body) {
  FIO_LOG_DEBUG("parse_put_article_body");

  FIOBJ article_key = fiobj_str_new("article", 7);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ json_body = FIOBJ_INVALID;
  fiobj_json2obj(&json_body, body, strlen(body));

  FIOBJ article_body = fiobj_hash_get(json_body, article_key);

  PutArticlePayload values;

  values.title = parse_optional_string(article_body, "title");
  values.description = parse_optional_string(article_body, "description");
  values.body = parse_optional_string(article_body, "body");
  values.tags = parse_optional_array(article_body, "tagList");

  fiobj_free(article_key);

  return values;
}