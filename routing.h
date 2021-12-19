//
// Created by hidetomo8111f on 2021/12/19.
//

#ifndef COCKTAIL_ROUTING_H
#define COCKTAIL_ROUTING_H

#include <search.h>

ENTRY* init_routing(ctRoute*,int);

void handle_request(ENTRY* routes,char* path, unsigned char* buf, char* responseBody, char* header,char* body, int readSock, int writeSock);

#endif //COCKTAIL_ROUTING_H
