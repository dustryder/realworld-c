#include "fio_cli.h"
#include "main.h"
#include "handlers/user/user_handlers.h"
#include "handlers/profiles/profile_handlers.h"
#include "handlers/articles/articles_handlers.h"
#include "../lib/router.h"
#include "../lib/middleware.h"

void _set_header(http_s *h, char *key, char *value) {
  http_set_header(h, fiobj_str_new(key, strlen(key)), fiobj_str_new(value, strlen(value)));
}

static void on_http_request(http_s *h) {

  http_parse_body(h);
  http_parse_query(h);

  _set_header(h, "content-type", "application/json");

  //user routes
  http_route_post(h, "/api/users", handle_post_user, resolve_request_user);
  http_route_post(h, "/api/users/login", handle_post_login, resolve_request_user);
  http_route_get(h, "/api/user", handle_get_user, resolve_request_user, require_auth);
  http_route_put(h, "/api/user", handle_put_user, resolve_request_user, require_auth);

  //profile routes
  http_route_get(h, "/api/profiles/:username", handle_get_profile, resolve_request_user);
  http_route_post(h, "/api/profiles/:username/follow", handle_post_follow, resolve_request_user, require_auth);
  http_route_delete(h, "/api/profiles/:username/follow", handle_delete_follow, resolve_request_user, require_auth);

  //articles routes
  http_route_post(h, "/api/articles", handle_post_articles, resolve_request_user, require_auth);
  http_route_get(h, "/api/articles/:slug", handle_get_articles, resolve_request_user, require_auth);

  http_send_error(h, 404);
}

/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {
  /* listen for inncoming connections */
  // FIO_LOG_LEVEL = FIO_LOG_LEVEL_DEBUG;

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
