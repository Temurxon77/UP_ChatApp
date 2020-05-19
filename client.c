#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "client.h"

void *recvmg(void *my_sock)
{
    int sock = *((int *)my_sock);
    int len;
    while((len = recv(sock,message,MAXSIZE,0)) > 0) {
        message[len] = '\0';
        fputs(message,stdout);
    }
}

int main(int argc,char *argv[]){
    pthread_t recvt;
    int len;
    int Newsocket;
    char* send_msg[MAXSIZE];
    struct sockaddr_in ServerIP;
    char* client_name[100];
    strcpy(client_name, argv[1]);
    Newsocket = socket( AF_INET, SOCK_STREAM,0);
    ServerIP.sin_port = htons(PORT);
    ServerIP.sin_family= AF_INET;
    ServerIP.sin_addr.s_addr = inet_addr("127.0.0.1");
    if( (connect( Newsocket ,(struct sockaddr *)&ServerIP,sizeof(ServerIP))) == -1 ){
        printf("\n connection to socket failed \n");
    }
    pthread_create(&recvt,NULL,(void *)recvmg,&Newsocket);
    while(fgets(message,MAXSIZE,stdin) > 0) {
        strcpy(send_msg,client_name);
        strcat(send_msg,":");
        strcat(send_msg,message);
        len = write(Newsocket,send_msg,strlen(send_msg));
        if(len < 0) {
            printf("\n message sent error! \n");
        }
    }
    pthread_join(recvt,NULL);
    close(Newsocket);
    return 0;
}