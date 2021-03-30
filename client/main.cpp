#include<iostream>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include<string.h>

using namespace std;

int main(){
    int listenfd;

    if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0)
        cout<<"socket"<<endl;
    
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        cout<<"bind"<<endl;

    char sendbuf[1024];
    char recvbuf[1024];
    while(fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL){
        send(listenfd,sendbuf,sizeof(sendbuf),0);
        recv(listenfd,recvbuf,sizeof(recvbuf),0);
        fputs(sendbuf,stdout);
        memset(recvbuf,0,sizeof(recvbuf));
        memset(sendbuf,0,sizeof(sendbuf));

    }
    close(listenfd);

    return 0;
    
    

}