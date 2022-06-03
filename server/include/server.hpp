#pragma once
#include <asyncd/asyncd.h>
#include "options.hpp"

int http_get_handler(short event, ad_conn_t *conn, void *userdata);


int http_default_handler(short event, ad_conn_t *conn, void *userdata);

int start_server(Options options);