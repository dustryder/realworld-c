#ifndef LIB_TOKEN_H
#define LIB_TOKEN_H
char* sign_jwt();
int decode_jwt_sub(char* token);
#endif