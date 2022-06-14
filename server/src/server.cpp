#include "pch.hpp"
#include "server.hpp"
#include "database.hpp"

int http_get_handler(short event, ad_conn_t *conn, void *userdata) {
    

    if (ad_http_get_status(conn) == AD_HTTP_REQ_DONE) {
        ad_http_t *http = (ad_http_t *) ad_conn_get_extra(conn);
        LOG_F(INFO, "URI: %s", http->request.uri);
        
        //get the utm data
        utmdata d(std::string(http->request.uri));
        //get the reply json
        auto json = Database::Get()->GetFilteredList(d);
        std::stringstream ss;
        ss << std::setw(4) << json << std::endl;
        std::string response = ss.str();

        ad_http_response(conn, 200, "text/json", response.data(), response.size()+1);
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

int start_server(Options options){
    ad_log_level(AD_LOG_DISABLE);
    ad_server_t *server = ad_server_new();
    LOG_S(INFO) << "Starting server on port: " << options.port;
    ad_server_set_option(server, "server.port", options.port.c_str());
    ad_server_set_option(server, "server.ssl_cert", "ssl.cert");
    ad_server_set_option(server, "server.ssl_pkey", "ssl.pkey");
    ad_server_register_hook(server, ad_http_handler, NULL); // HTTP Parser is also a hook.
    ad_server_register_hook_on_method(server, "GET", http_get_handler, NULL);
    ad_server_register_hook(server, http_default_handler, NULL);
    return ad_server_start(server);
}