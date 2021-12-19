#ifndef COCKTAIL_COCKTAIL_H
#define COCKTAIL_COCKTAIL_H

#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define METHOD_PUT "PUT"
#define METHOD_DELETE "DELETE"

#define CTX_JSON "Content-type: json\n"
#define CTX_HTML "Content-type: text/html\n"

#define PAGE_ERR "<h1>404 Not Found</h1>\n"
#define STATUS_OK "HTTP/1.0 200 OK\n"
#define STATUS_NOT_FOUND "HTTP/1.0 404 Not Found\n"

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
