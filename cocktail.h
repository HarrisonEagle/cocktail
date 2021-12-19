#ifndef COCKTAIL_COCKTAIL_H
#define COCKTAIL_COCKTAIL_H

#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define METHOD_PUT "PUT"
#define METHOD_DELETE "DELETE"

#define CTX_JSON "Content-type: json\n"
#define CTX_HTML "Content-type: text/html\n"

#define STATUS_CONTINUE "HTTP/1.0 100 Continue\n"

#define STATUS_OK "HTTP/1.0 200 OK\n"

#define STATUS_MOVED_PERMANEMTLY "HTTP/1.0 301 Moved Permanently\n"
#define STATUS_MOVED_FOUND "HTTP/1.0 302 Found\n"

#define STATUS_BAD_REQUEST "HTTP/1.0 400 Bad Request\n"
#define STATUS_UNAUTHORIZED "HTTP/1.0 401 Unauthorized\n"
#define STATUS_PAYMENT_REQUIRED "HTTP/1.0 402 Payment Required\n"
#define STATUS_FORBIDDEN "HTTP/1.0 403 Forbidden\n"
#define STATUS_NOT_FOUND "HTTP/1.0 404 Not Found\n"
#define STATUS_METHOD_NOT_ALLOWED "HTTP/1.0 405 Method Not Allowed\n"
#define STATUS_NOT_ACCEPTABLE "HTTP/1.0 406 Not Acceptable\n"

#define STATUS_INTERNAL_SERVER_ERROR "HTTP/1.0 500 Internal Server Error\n"
#define STATUS_NOT_IMPLEMENTED "HTTP/1.0 501 Not Implemented\n"
#define STATUS_SERVICE_UNAVAILABLE "HTTP/1.0 502 Service Unavailable\n"
#define STATUS_GATEWAY_TIMEOUT "HTTP/1.0 503 Gateway Timeout\n"
#define STATUS_NOT_SUPPORTED "HTTP/1.0 504 HTTP Version Not Supported\n"

typedef struct ctContext{
    char* contentBody;
} ctContext;

typedef struct ctResponse{
    char* contentType;
    char* status;
    char* responseBody;
} ctResponse;

typedef struct ctRoute{
    char* path;
    char* method;
    ctResponse (* ctHandler)(ctContext);
}ctRoute;

void start_server(int,ctRoute[],int);

#endif //COCKTAIL_COCKTAIL_H
