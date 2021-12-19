
#include <stdio.h>
#include "cocktail.h"
#include "routing.h"
#include "hashmap.h"

hashmap* init_routing(ctRoute *ctroutes,int routesize){
    hashmap* routes = hashmap_create();
    fprintf(stdout,"Initializing Server...\n");
    for(int i=0;i<routesize;i++){
        fprintf(stdout,"%s %s\n",ctroutes[i].method,ctroutes[i].path);
        routing_set(routes,ctroutes[i].path,&ctroutes[i]);
    }
    return routes;
}