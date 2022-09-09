#include <evhttp.h>

void process_request(struct evhttp_request *req, void *arg){ 
    fprintf(stdout, "Request!\n");
    struct evbuffer *buf = evbuffer_new();
    if (buf == NULL) return;
    evbuffer_add_printf(buf, "Requested: %s\n", evhttp_request_uri(req));
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

int main () {

    struct event_base *base = NULL;
    struct evhttp *httpd = NULL;
    base = event_init();
    if (base == NULL) return -1;
    httpd = evhttp_new(base);
    if (httpd == NULL) return -1;
    
    if (evhttp_bind_socket(httpd, "0.0.0.0", 3000) != 0) return -1;
    fprintf(stdout, "Server running at 3000\n");

    evhttp_set_gencb(httpd, process_request, NULL);
    event_base_dispatch(base);
    return 0;
}