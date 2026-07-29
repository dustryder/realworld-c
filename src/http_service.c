#include "fio_cli.h"
#include "main.h"
#include "handlers/user_handler.h"

/* TODO: edit this function to handle HTTP data and answer Websocket requests.*/
static void on_http_request(http_s *h) {

  printf("%s\n", fiobj_obj2cstr(h->method).data);
  printf("%s\n", fiobj_obj2cstr(h->path).data);

  if (http_parse_body(h) == -1) {
      http_send_error(h, 400);
      return;
  }

  handle_post_user(h);
  http_send_body(h, "Hello World!", 12);
}

/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {
  /* listen for inncoming connections */
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
