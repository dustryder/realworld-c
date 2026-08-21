#include "main.h"
#include "articles_handlers.h"
#include "cJSON.h"
#include "../../services/articles/articles_services.h"
#include "../../lib/constants.h"

static PostArticlePayload parse_post_article_body(FIOBJ *raw_body);

void handle_post_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_post_profile");

    int id = parse_request_user(h->params);
    char* response_body;

    PostArticlePayload payload = parse_post_article_body(h->body);

    CreateArticleResult result = create_article(h->udata, id, payload.title, payload.description, payload.body, payload.tags, payload.tag_count);

    response_body = create_article_success_response(result.result, true, FORMAT_DATESTAMP);
    h->status = HTTP_CREATED;

    free(payload.tags);

    http_send_body(h, response_body, strlen(response_body));
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

  FIOBJ taglist = fiobj_hash_get(article_body, taglist_key);

  char **tags = malloc(sizeof(char *) * fiobj_ary_count(taglist));

  if (FIOBJ_TYPE_IS(taglist, FIOBJ_T_ARRAY)) {
    for (int i = 0; i < (int) fiobj_ary_count(taglist); i++) {
        FIOBJ item = fiobj_ary_index(taglist, i);
        char *string_item = fiobj_obj2cstr(item).data;
        tags[i] = string_item;
    }
  }

  values.tags = tags;
  values.tag_count = fiobj_ary_count(taglist);

  fiobj_free(article_key);
  fiobj_free(title_key);
  fiobj_free(description_key);
  fiobj_free(body_key);
  fiobj_free(taglist_key);

  return values;
}