#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "server.h"

pthread_mutex_t mutex; // locker for Semaphore multithreading
void sendtoall(char *msg,int curr){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < n; i++) {
        if(clients[i] != curr) {
            if(send(clients[i],msg,strlen(msg),0) < 0) {
                printf("Fail to send... \n");
                continue;
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *recvmg(void *client_sock){
    int sock = *((int *)client_sock);
    char* msg[MAXSIZE];
    int len;
    while((len = recv(sock,msg,MAXSIZE,0)) > 0) {
        msg[len] = '\0';
        sendtoall(msg,sock);
    }

}
int main(){
    struct sockaddr_in ServerIp;
    pthread_t recvt;
    int sock=0 , Client_sock=0;

    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(5000);
    ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock = socket( AF_INET , SOCK_STREAM, 0 );
    if( bind( sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1 )
        printf("Failed to bind... \n");
    else
        printf("Connection Established\n Server Started on port: %d\n",PORT);

    if( listen( sock ,20 ) == -1 )
        printf("Failed to listen... \n");

    while(1){
        if( (Client_sock = accept(sock, (struct sockaddr *)NULL,NULL)) < 0 )
            printf("Failed to accept...  \n");
        pthread_mutex_lock(&mutex);
        clients[n]= Client_sock;
        n++;
        // creating a thread for each client 
        pthread_create(&recvt,NULL,(void *)recvmg,&Client_sock);
        pthread_mutex_unlock(&mutex);
    }
    return 0; 

}

