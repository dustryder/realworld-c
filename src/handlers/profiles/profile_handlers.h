#include "main.h"
#include "../../services/profiles/profiles_services.h"

#ifndef PROFILE_HANDLERS_H
#define PROFILE_HANDLERS_H
void handle_get_profile(http_s* h);
void handle_post_follow(http_s* h);
void handle_delete_follow(http_s* h);

char *create_success_profile_response(ProfileServiceResultData data);
#endif