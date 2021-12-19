#include "cocktail.h"
#include "routing.h"
#include "hashmap.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>
#include <malloc.h>
#include <sys/wait.h>
#include <stdlib.h>

static volatile int keepRunning = 1;
unsigned int childProcCount = 0;

void exitServer(int signal) {
    fprintf(stdout,"Server Process Terminated!.");
    keepRunning = 0;
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void ChildExitSignalHandler()
{
    pid_t processID;           /* Process ID from fork() */

    while (childProcCount) /* Clean up all zombies */
    {
        processID = waitpid((pid_t) -1, NULL, WNOHANG);  /* Non-blocking wait */
        if (processID < 0)  /* waitpid() error? */
            DieWithError("waitpid() failed");
        else if (processID == 0)  /* No child to wait on */
            break;
        else
            childProcCount--;  /* Cleaned up after a child */
    }
}

int setup_socket(int port){
    struct sockaddr_in addr;
    int readSock = socket(AF_INET, SOCK_STREAM, 0);

    if (readSock < 0) {
        DieWithError("Error. Cannot make socket\n");
    }

    /* socket setting */
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    //binding socket
    int ret = bind(readSock, (struct sockaddr *)&addr, sizeof(addr));

    if (ret < 0) {
        DieWithError("Error. Cannot bind socket\n");
    }

    signal(SIGINT, exitServer);
    //listen socket
    if(listen(readSock, 5) < 0){
        DieWithError("Error. Cannot bind socket\n");
    }
    fprintf(stdout,"Listeing port at %d\n",port);
    return readSock;
}

void start_server(int port,ctRoute *ctroutes,int routesize){
    int readSock, writeSock;
    char path[1024];
    unsigned char buf[1024];
    char responseBody[1024];
    struct sockaddr_in client;
    struct sigaction signal_handler;
    int len;
    hashmap* routes = init_routing(ctroutes,routesize);

    pid_t processID;
    /* make socket */
    readSock = setup_socket(port);

    signal_handler.sa_handler =  ChildExitSignalHandler;
    if (sigfillset(&signal_handler.sa_mask) < 0) {  /* mask all signals */
        DieWithError("sigfillset() failed");
    }
    signal_handler.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &signal_handler, 0) < 0){
        DieWithError("sigfillset() failed");
    }
    /* accept TCP connection from client */
    while (keepRunning){
        len = sizeof(client);
        //handshake with socket

        writeSock = accept(readSock, (struct sockaddr *)&client, &len);
        if ((processID = fork()) < 0){
            DieWithError("create child process failed");
        }else if (processID == 0){
            handle_request(routes,path,buf,responseBody,readSock,writeSock);
        }
        //close write socket
        close(writeSock);
        childProcCount++;
    }
}
