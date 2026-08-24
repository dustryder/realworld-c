#include "main.h"
#include "articles_handlers.h"
#include "../../lib/http_helpers.h"
#include "../../lib/constants.h"
#include "../../lib/type.h"

static PutArticlePayload parse_PutArticlePayload(FIOBJ *raw_body);
static void free_PutArticlePayload(PutArticlePayload payload);
static void validate_PutArticlePayload(PutArticlePayload payload, ErrorValue *values, size_t *error_count);

void handle_put_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_put_article");

    int id = parse_request_user(h->params);
    char* response_body = NULL;

    ErrorValue errors[4];
    size_t error_count = 0;

    PutArticlePayload payload = parse_PutArticlePayload(h->body);
    validate_PutArticlePayload(payload, errors, &error_count);
    char *slug = parse_path_param(h->params, "slug");

    if (error_count > 0) {
      response_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      ArticleServiceResult result = update_article(
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
        free_ArticlesServiceResultData(&result.result);
      } else if (result.status == SERVICE_NOT_FOUND) {
        response_body = create_failure_body_from_error(result.error);
        h->status = HTTP_NOT_FOUND;
      } else if (result.status == SERVICE_UNAUTHORIZED) {
        response_body = create_failure_body_from_error(result.error);
        h->status = HTTP_FORBIDDEN;
      } else {
        response_body = create_empty_response();
        h->status = HTTP_INTERNAL_SERVER_ERROR;
      }
    }

    http_send_body(h, response_body, strlen(response_body));

    free_PutArticlePayload(payload);
    free(response_body);
    free(slug);
}

void validate_PutArticlePayload(PutArticlePayload payload, ErrorValue *values, size_t *error_count) {

  if (payload.title.is_present && payload.title.value == NULL) {
    values[*error_count].property = "title";
    values[*error_count].message = "can't be null";
    (*error_count)++;
  }

  if (payload.body.is_present && payload.body.value == NULL) {
    values[*error_count].property = "body";
    values[*error_count].message = "can't be null";
    (*error_count)++;
  }

  if (payload.description.is_present && payload.description.value == NULL) {
    values[*error_count].property = "description";
    values[*error_count].message = "can't be null";
    (*error_count)++;
  }

  if (payload.tags.is_present && payload.tags.value == NULL) {
    values[*error_count].property = "tagList";
    values[*error_count].message = "can't be null";
    (*error_count)++;
  }
}

void free_PutArticlePayload(PutArticlePayload payload) {

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

PutArticlePayload parse_PutArticlePayload(FIOBJ *raw_body) {
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