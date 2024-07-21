#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include "constants.h"

struct client_request {
  char* method_type;
  char* route;
  char* http_version;

};

struct request_header {
  char* name;
  char* value;
};

client_request* parse_request(char[BUFFER_SIZE]);

#endif // REQUEST_PARSER_H
