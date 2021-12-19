
#include <stdio.h>
#include <search.h>
#include <stdlib.h>
#include "cocktail.h"
#include "routing.h"

ENTRY* init_routing(ctRoute *ctroutes,int routesize){
    ENTRY e;
    ENTRY *routes = hcreate(routesize);
    fprintf(stdout,"Initializing Server...\n");
    for(int i=0;i<routesize;i++){
        fprintf(stdout,"%s %s\n",ctroutes[i].method,ctroutes[i].path);
        e.key = ctroutes[i].path;
        e.data = (void *) &ctroutes[i];
        routes = hsearch(e, ENTER);
        if (routes == NULL) {
            fprintf(stderr, "entry failed\n");
            exit(1);
        }
    }
    return routes;
}