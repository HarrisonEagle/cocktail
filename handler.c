#include <string.h>
#include <sys/socket.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include "cocktail.h"
#include "routing.h"

void handle_request(hashmap* routes,char* path, unsigned char* buf, char* responseBody, int readSock, int writeSock){
    memset(path, 0, 1024);
    memset(responseBody, 0, 1024);
    //recv socket
    int recv_len = recv(writeSock, buf,1024, 0);
    buf[recv_len - 1] = 0;
    if (buf[0] == '\0'){
        strcpy(buf, NULL);
    }

    // method
    for (int i = 0; i < strlen(buf); i++) {
        if (buf[i] == 'G' && buf[i + 1] == 'E' && buf[i + 2] == 'T' && buf[i + 3] == ' ') {
            for (int j = 0; buf[i + 4 + j] != ' '; j++) {
                path[j] = buf[i + 4 + j];
            }
            break;
        }else if (buf[i] == 'P' && buf[i + 1] == 'O' && buf[i + 2] == 'S' && buf[i + 3] == 'T' && buf[i + 4] == ' ') {
            for (int j = 0; buf[i + 4 + j] != ' '; j++) {
                path[j] = buf[i + 4 + j];
            }
            break;
        }
    }
    unsigned char *header;
           // "HTTP/1.0 200 OK\n"
                        //    "Content-type: text/html\n" //"Content-type: text/html\n"
    unsigned char *body;//"<h1>Hello World!</h1>\n"
                        // "<h1>This is Test Response</h1>\n";
    //respond with send
    printf("path: %s \n",path);
    ctRoute* ctroute = routing_get(routes,path);
    if(ctroute!=NULL){
        ctContext context = {responseBody};
        ctroute->ctHandler(context);
    }else{
        fprintf(stderr,STATUS_NOT_FOUND);
        header = (char *)malloc((int)STATUS_NOT_FOUND+(int)CTX_HTML+1);
        strcat(header,STATUS_NOT_FOUND);
        strcat(header,CTX_HTML);
        fprintf(stdout,header);
        body = (char *) malloc(PAGE_ERR);
        strcat(body,PAGE_ERR);
    }
    strcat(header,"\n");
    unsigned char *response = malloc(strlen(header)+ strlen(body)+1);
    strcat(response ,header);
    strcat(response,body);
    printf("Resp:%s\n",response);
    send(writeSock, response, strlen(response), 0);
    close(readSock);
    exit(0);
}


