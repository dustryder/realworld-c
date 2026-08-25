#include "fio_cli.h"
#include "main.h"
#include "./aio.h"
#include "./handlers/profiles/profile_handlers.h"
#include "./handlers/articles/articles_handlers.h"
#include "./handlers/tags/tags_handlers.h"
#include "./handlers/comments/comments_handlers.h"
#include "./lib/router.h"
#include "./lib/middleware.h"
#include "./lib/constants.h"

void set_header(http_s *h, char *key, char *value) {
    http_set_header(h, fiobj_str_new(key, strlen(key)), fiobj_str_new(value, strlen(value)));
}

PGconn *set_db_connection(http_s *h) {
  PGconn *connection = get_connection();
  h->udata = connection;

  return connection;
}

char *handle_options(http_s *h) {
  if (strcmp(fiobj_obj2cstr(h->method).data, "OPTIONS") == 0) {
    set_header(h, "Allow", "OPTIONS, GET, POST, PUT, DELETE");
    set_header(h, "Access-Control-Allow-Origin", "*");
    set_header(h, "Access-Control-Allow-Methods", "OPTIONS, GET, POST, PUT, DELETE");

    h->status = HTTP_NO_CONTENT;
    http_send_body(h, "", 0);
  }
}

static void on_http_request(http_s *h) {
  http_parse_body(h);
  http_parse_query(h);
  handle_options(h);

  PGconn *conn = set_db_connection(h);
  set_header(h, "content-type", "application/json");

  //user routes
  http_route_post(h, "/api/users", handle_post_user, resolve_request_user);
  http_route_post(h, "/api/users/login", handle_post_login, resolve_request_user);
  http_route_get(h, "/api/user", handle_get_user, resolve_request_user, require_auth);
  http_route_put(h, "/api/user", handle_put_user, resolve_request_user, require_auth);

  //profile routes
  http_route_get(h, "/api/profiles/:username", handle_get_profile, resolve_request_user);
  http_route_post(h, "/api/profiles/:username/follow", handle_post_follow, resolve_request_user, require_auth);
  http_route_delete(h, "/api/profiles/:username/follow", handle_delete_follow, resolve_request_user, require_auth);

  //articles
  http_route_get(h, "/api/articles/feed", handle_get_all_feed, resolve_request_user, require_auth);
  http_route_post(h, "/api/articles", handle_post_articles, resolve_request_user, require_auth);
  http_route_get(h, "/api/articles/:slug", handle_get_articles, resolve_request_user);
  http_route_get(h, "/api/articles", handle_get_all_articles, resolve_request_user);
  http_route_put(h, "/api/articles/:slug", handle_put_articles, resolve_request_user, require_auth);
  http_route_delete(h, "/api/articles/:slug", handle_delete_articles, resolve_request_user, require_auth);

  // article favorites
  http_route_post(h, "/api/articles/:slug/favorite", handle_post_favorite, resolve_request_user, require_auth);
  http_route_delete(h, "/api/articles/:slug/favorite", handle_delete_favorite, resolve_request_user, require_auth);

  //article comments
  http_route_get(h, "/api/articles/:slug/comments", handle_get_comments, resolve_request_user);
  http_route_post(h, "/api/articles/:slug/comments", handle_post_comments, resolve_request_user, require_auth);
  http_route_delete(h, "/api/articles/:slug/comments/:id", handle_delete_comments, resolve_request_user, require_auth);

  //tag
  http_route_get(h, "/api/tags", handle_get_tags, resolve_request_user);

db_cleanup:
  PQfinish(conn);

  http_send_error(h, 404);
}

/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {
  /* listen for inncoming connections */
  FIO_LOG_LEVEL = FIO_LOG_LEVEL_DEBUG;

  if (http_listen(fio_cli_get("-p"), fio_cli_get("-b"),
                  .on_request = on_http_request,
                  .max_body_size = fio_cli_get_i("-maxbd") * 1024 * 1024,
                  .ws_max_msg_size = fio_cli_get_i("-max-msg") * 1024,
                  .public_folder = fio_cli_get("-public"),
                  .log = fio_cli_get_bool("-log"),
                  .timeout = fio_cli_get_i("-keep-alive"),
                  .ws_timeout = fio_cli_get_i("-ping")) == -1) {
    /* listen failed ?*/
    perror("ERROR: facil couldn't initialize HTTP service (already running?)");
    exit(1);
  }
}
