#include "main.h"
#include "articles_handlers.h"
#include "cJSON.h"
#include "../../services/articles/articles_services.h"
#include "../../lib/constants.h"

static PostArticlePayload parse_post_article_body(FIOBJ *raw_body);

void handle_post_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_post_articles");

    int id = parse_request_user(h->params);
    char* response_body;
    ErrorValue errors[3];
    size_t error_count = 0;

    PostArticlePayload payload = parse_post_article_body(h->body);
    validate_article_payload(payload, errors, &error_count);

    if (error_count > 0) {
      response_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      CreateArticleResult result = create_article(h->udata, id, payload.title, payload.description, payload.body, payload.tags);

      response_body = create_article_success_response(result.result, true, FORMAT_DATESTAMP);
      h->status = HTTP_CREATED;
    }

    if (payload.tags.value != NULL) free(payload.tags.value);

    http_send_body(h, response_body, strlen(response_body));
}

void validate_article_payload(PostArticlePayload payload, ErrorValue *values, size_t *error_count) {

  if (strlen(payload.title) == 0) {
    values[*error_count].property = "title";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }

  if (strlen(payload.body) == 0) {
    values[*error_count].property = "body";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }

  if (strlen(payload.description) == 0) {
    values[*error_count].property = "description";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }
}

PostArticlePayload parse_post_article_body(FIOBJ *raw_body) {

  FIOBJ article_key = fiobj_str_new("article", 7);

  FIOBJ title_key = fiobj_str_new("title", 5);
  FIOBJ description_key = fiobj_str_new("description", 11);
  FIOBJ body_key = fiobj_str_new("body", 4);
  FIOBJ taglist_key = fiobj_str_new("tagList", 7);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ json_body = FIOBJ_INVALID;
  fiobj_json2obj(&json_body, body, strlen(body));

  FIOBJ article_body = fiobj_hash_get(json_body, article_key);

  PostArticlePayload values;

  values.title = fiobj_obj2cstr(fiobj_hash_get(article_body, title_key)).data;
  values.description = fiobj_obj2cstr(fiobj_hash_get(article_body, description_key)).data;
  values.body = fiobj_obj2cstr(fiobj_hash_get(article_body, body_key)).data;
  values.tags = parse_optional_array(article_body, "tagList");

  fiobj_free(article_key);
  fiobj_free(title_key);
  fiobj_free(description_key);
  fiobj_free(body_key);
  fiobj_free(taglist_key);

  return values;
}