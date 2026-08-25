#include "main.h"
#include "../../lib/constants.h"
#include "comments_handlers.h"
#include "../../services/comments/comments_services.h"

static PostCommentPayload parse_PostCommentPayload(FIOBJ raw_body);
void validate_PostCommentPayload(PostCommentPayload payload, ErrorValue *values, size_t *error_count);

void handle_post_comments(http_s *h) {
    FIO_LOG_DEBUG("handle_post_comments");

    int id = parse_request_user(h->params);
    char *slug = parse_path_param(h->params, "slug");
    PostCommentPayload payload = parse_PostCommentPayload(h->body);
    ErrorValue errors[3];
    size_t error_count = 0;
    validate_PostCommentPayload(payload, errors, &error_count);

    char *response_body = NULL;

    if (error_count > 0) {
      response_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
        CommentsServiceResult service_result = create_comment(h->udata, slug, id, payload.body);

        if (service_result.status == SERVICE_SUCCESS) {
          response_body = create_comment_success_response(service_result.result);
          h->status = HTTP_CREATED;
        } else if (service_result.status == SERVICE_NOT_FOUND) {
          response_body = create_failure_body_from_error(service_result.error);
          h->status = HTTP_NOT_FOUND;
        } else {
          response_body = create_empty_response();
          h->status = HTTP_INTERNAL_SERVER_ERROR;
        }
        free_CommentsServiceResultData(&service_result.result);
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
    free(payload.body);
    free(slug);
}

void validate_PostCommentPayload(PostCommentPayload payload, ErrorValue *values, size_t *error_count) {
  if (strlen(payload.body) == 0) {
    values[*error_count].property = "body";
    values[*error_count].message = "can't be blank";
    (*error_count)++;
  }
}

PostCommentPayload parse_PostCommentPayload(FIOBJ raw_body) {

  FIOBJ comment_key = fiobj_str_new("comment", 7);

  FIOBJ body_key = fiobj_str_new("body", 4);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ json_body = FIOBJ_INVALID;
  fiobj_json2obj(&json_body, body, strlen(body));

  FIOBJ comment_body = fiobj_hash_get(json_body, comment_key);

  PostCommentPayload values;

  values.body = strdup(fiobj_obj2cstr(fiobj_hash_get(comment_body, body_key)).data);

  fiobj_free(body_key);
  fiobj_free(comment_key);
  fiobj_free(json_body);

  return values;
}