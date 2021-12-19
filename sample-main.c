#include <bits/types/FILE.h>
#include <stddef.h>
#include <stdio.h>
#include "cocktail.h"

ctResponse method1(ctContext context){
    ctResponse response = {
            CTX_HTML,
            STATUS_OK,
            "<!DOCTYPE html>\n"
            "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"
            "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,maximum-scale=1\">\n"
            "</head>\n"
            "<body>\n"
            "</div>\n"
            "<div class=\"main\">\n"
            "<p class=\"bigtitle\">Programming Language</p>\n"
            "<ul class=\"ul_class\">\n"
            "<li class=\"li_class\">Java</li>\n"
            "<li class=\"li_class\">Python</li>\n"
            "<li class=\"li_class\">C/C++</li>\n"
            "<li class=\"li_class\">Ruby</li>\n"
            "<li class=\"li_class\">PHP</li>\n"
            "<li class=\"li_class\">HTML&amp;CSS</li>\n"
            "<li class=\"li_class\">javaScript</li>\n"
            "<li class=\"li_class\">TypeScript</li>\n"
            "<li class=\"li_class\">jQuery</li>\n"
            "<li class=\"li_class\">C#</li>\n"
            "<li class=\"li_class\">Go</li>\n"
            "<li class=\"li_class\">scipy</li>\n"
            "<li class=\"li_class\">Kotlin</li>\n"
            "</ul>\n"
            "\n"
            "</div>\n"
            "</body></html>"
    };
    return response;
}

ctResponse method2(ctContext context){
    const char *file = "blog1.html";
    FILE *fp;
    fp = fopen(file, "r");
    if (fp == NULL)
    {
        ctResponse failedResponse = {
                CTX_HTML,
                STATUS_INTERNAL_SERVER_ERROR,
                "<p>Init Blog Failed.</p>"
        };
        return failedResponse;
    }

    fclose(fp);
    ctResponse response = {
            CTX_HTML,
            STATUS_OK,
            "<p>Init Blog Succeed.</p>"
    };
    return response;
}

ctResponse method3(ctContext context){
    ctResponse response = {
            CTX_JSON,
            STATUS_OK,
            "{\"message\":\"This is test Post route.\"}"
    };
    return response;
}

int main(int argc, char *argv[]){
    ctRoute ctroutes[] = {
            {"/index",METHOD_GET,method1},
            {"/works/blog",METHOD_GET,method2},
            {"/help",METHOD_POST,method3},
    };
    start_server(8080,ctroutes,3);
}

