#include "main.h"
#include "articles_handlers.h"
#include "cJSON.h"
#include "../../services/articles/articles_services.h"
#include "../../lib/constants.h"

static PostArticlePayload parse_PostArticlePayload(FIOBJ *raw_body);
static void free_PostArticlePayload(PostArticlePayload payload);
static void validate_PostArticlePayload(PostArticlePayload payload, ErrorValue *values, size_t *error_count);

void handle_post_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_post_articles");
    int id = parse_request_user(h->params);
    char* response_body = NULL;
    ErrorValue errors[3];
    size_t error_count = 0;

    PostArticlePayload payload = parse_PostArticlePayload(h->body);
    validate_PostArticlePayload(payload, errors, &error_count);

    if (error_count > 0) {
      response_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      ArticleServiceResult result = create_article(
        h->udata,
        id,
        payload.title.value,
        payload.description.value,
        payload.body.value,
        payload.tags
      );

      if (result.status == SERVICE_SUCCESS) {
        response_body = create_article_success_response(result.result, true, FORMAT_DATESTAMP);
        h->status = HTTP_CREATED;
      } else {
        response_body = create_empty_response();
        h->status == HTTP_INTERNAL_SERVER_ERROR;
      }

      free_ArticlesServiceResultData(&result.result);
    }

    http_send_body(h, response_body, strlen(response_body));

    free_PostArticlePayload(payload);
    free(response_body);
}

void validate_PostArticlePayload(PostArticlePayload payload, ErrorValue *values, size_t *error_count) {

  if (!(payload.title.is_present)) {
    values[*error_count].property = "title";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }

  if (payload.title.is_present && (payload.title.value == NULL || strlen(payload.title.value) == 0)) {
    values[*error_count].property = "title";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }

  if (!(payload.body.is_present)) {
    values[*error_count].property = "body";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }

  if (payload.body.is_present && (payload.body.value == NULL || strlen(payload.body.value) == 0)) {
    values[*error_count].property = "body";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }

  if (!(payload.description.is_present)) {
    values[*error_count].property = "description";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }

  if (payload.description.is_present && (payload.description.value == NULL || strlen(payload.description.value) == 0)) {
    values[*error_count].property = "description";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }
}

void free_PostArticlePayload(PostArticlePayload payload) {
  free(payload.body.value);
  free(payload.description.value);
  free(payload.title.value);

  if (payload.tags.value != NULL) {
    for(int i = 0; i < payload.tags.value_count; i++) {
      free(payload.tags.value[i]);
    }

    free(payload.tags.value);
  }
}

PostArticlePayload parse_PostArticlePayload(FIOBJ *raw_body) {

  FIOBJ article_key = fiobj_str_new("article", 7);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ json_body = FIOBJ_INVALID;
  fiobj_json2obj(&json_body, body, strlen(body));

  FIOBJ article_body = fiobj_hash_get(json_body, article_key);

  PostArticlePayload values;

  values.title = parse_optional_string(article_body, "title");
  values.description = parse_optional_string(article_body, "description");
  values.body = parse_optional_string(article_body, "body");
  values.tags = parse_optional_array(article_body, "tagList");

  fiobj_free(article_key);

  fiobj_free(json_body);

  return values;
}