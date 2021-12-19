//
// Created by hidetomo8111f on 2021/12/19.
//

#ifndef COCKTAIL_ROUTING_H
#define COCKTAIL_ROUTING_H

#include "hashmap.h"

hashmap* init_routing(ctRoute*,int);

void handle_request(hashmap* routes,char* path, unsigned char* buf, char* responseBody, int readSock, int writeSock);

#endif //COCKTAIL_ROUTING_H
