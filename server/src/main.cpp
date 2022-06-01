#include "pch.hpp"
#include <asyncd/asyncd.h>



int http_get_handler(short event, ad_conn_t *conn, void *userdata) {
    
    ad_http_t *http = (ad_http_t *) ad_conn_get_extra(conn);
    LOG_F(INFO, "URI: %s", http->request.uri);

    if (ad_http_get_status(conn) == AD_HTTP_REQ_DONE) {
        
        ad_http_response(conn, 200, "text/html", "Hello World", 11);
        return AD_DONE; // Keep connection alive.
    }

    return AD_OK;
}


int http_default_handler(short event, ad_conn_t *conn, void *userdata) {
    if (ad_http_get_status(conn) == AD_HTTP_REQ_DONE) {
        ad_http_response(conn, 501, "text/html", "Not implemented", 15);
        return AD_CLOSE; // Close connection.
    }
    return AD_OK;
}

int main(int argc, char **argv) {
    loguru::init(argc, argv);
    loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
    loguru::add_file("latest_readable.log", loguru::Truncate, loguru::Verbosity_INFO);
    loguru::g_stderr_verbosity = 1;


    ad_log_level(AD_LOG_DISABLE);
    ad_server_t *server = ad_server_new();
    ad_server_set_option(server, "server.port", "8888");
    ad_server_set_option(server, "server.ssl_cert", "ssl.cert");
    ad_server_set_option(server, "server.ssl_pkey", "ssl.pkey");
    ad_server_register_hook(server, ad_http_handler, NULL); // HTTP Parser is also a hook.
    ad_server_register_hook_on_method(server, "GET", http_get_handler, NULL);
    ad_server_register_hook(server, http_default_handler, NULL);
    std::cout << "starting..." << std::endl;
    return ad_server_start(server);
}