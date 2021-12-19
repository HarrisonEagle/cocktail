#include <string.h>
#include <sys/socket.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <search.h>
#include "cocktail.h"
#include "routing.h"

void handle_request(ENTRY* routes,char* path, unsigned char* buf, char* responseBody,char* header,char* body,int readSock, int writeSock){
    memset(path, 0, 1024);
    memset(responseBody, 0, 1024);
    //recv socket
    int recv_len = recv(writeSock, buf,1024, 0);
    buf[recv_len - 1] = 0;
    // method
    char* method;
    for (int i = 0; i < strlen(buf); i++) {
        if (buf[i] == 'G' && buf[i + 1] == 'E' && buf[i + 2] == 'T' && buf[i + 3] == ' ') {
            for (int j = 0; buf[i + 4 + j] != ' '; j++) {
                path[j] = buf[i + 4 + j];
            }
            method = METHOD_GET;
            break;
        }else if (buf[i] == 'P' && buf[i + 1] == 'O' && buf[i + 2] == 'S' && buf[i + 3] == 'T' && buf[i + 4] == ' ') {
            for (int j = 0; buf[i + 5 + j] != ' '; j++) {
                path[j] = buf[i + 5 + j];
            }
            method = METHOD_POST;
            break;
        }
        else if (buf[i] == 'P' && buf[i + 1] == 'U' && buf[i + 2] == 'T' && buf[i + 3] == ' ') {
            for (int j = 0; buf[i + 4 + j] != ' '; j++) {
                path[j] = buf[i + 4 + j];
            }
            method = METHOD_PUT;
            break;
        }
        else if (buf[i] == 'D' && buf[i + 1] == 'E' && buf[i + 2] == 'L' && buf[i + 3] == 'E' && buf[i + 4] == 'T' && buf[i + 5] == 'E' &&buf[i + 6] == ' ') {
            for (int j = 0; buf[i + 7 + j] != ' '; j++) {
                path[j] = buf[i + 7 + j];
            }
            method = METHOD_DELETE;
            break;
        }
    }
    //respond with send
    printf("path: %s \n",path);
    ENTRY e;
    e.key = path;
    routes = hsearch(e,FIND);
    strcpy(header, "");
    strcpy(body, "");
    if(routes!=NULL){
        ctContext context = {responseBody};
        //ctroute->ctHandler(context);
        //ctRoute *ctroute = (ctRoute*)routes;
        if(strcmp(((ctRoute*)(routes->data))->method,method) == 0){
            ctResponse response = ((ctRoute*)(routes->data))->ctHandler(context);
            strcat(header,response.status);
            strcat(header,response.contentType);
            strcat(header,"\n");
            strcat(body,response.responseBody);
        }else{
            fprintf(stderr,STATUS_METHOD_NOT_ALLOWED);
            strcat(header,STATUS_METHOD_NOT_ALLOWED);
            strcat(header,CTX_HTML);
            strcat(header,"\n");
            strcat(body,"<h1>405 Method Not Aloowed.</h1>\n");
        }
    }else{
        fprintf(stderr,STATUS_NOT_FOUND);
        strcat(header,STATUS_NOT_FOUND);
        strcat(header,CTX_HTML);
        strcat(header,"\n");
        strcat(body,"<h1>404 Not Found</h1>\n");
    }
    char *response = malloc(strlen(header)+ strlen(body));
    strcat(response ,header);
    strcat(response,body);
    send(writeSock, response, strlen(response), 0);
    close(readSock);
    exit(0);
}


