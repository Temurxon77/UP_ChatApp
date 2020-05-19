#define PORT 5000
#define MAXSIZE 512

int clients[20];
int n=0;

void sendtoall(char*,int);

void *recvmg(void*);