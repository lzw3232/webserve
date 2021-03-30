#include<iostream>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

#include "./framework/ERR/ERR_EXIT.h"

using namespace std;
void serve(int conn){
    char recvbuf[1024]={0};
    while(1){
        memset(recvbuf,0,sizeof(recvbuf));
        int ret = recv(conn,recvbuf,sizeof(recvbuf),0);
        if(ret==0){
            cout<<"client close"<<endl;
            break;
        }
        fputs(recvbuf,stdout);
        send(conn,recvbuf,sizeof(recvbuf),0);
    }
}

int main(){
    int listenfd;
    if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0)
        ERR_EXIT("socket");
    
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        ERR_EXIT("bind");

    if(listen(listenfd,SOMAXCONN)<0)
        ERR_EXIT("listen");
    
    struct sockaddr_in peeraddr;
    memset(&peeraddr,0,sizeof(peeraddr));
    socklen_t peerlen = sizeof(peeraddr);
    int conn;

    pid_t pid;
    while(1){
        if((conn = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
            ERR_EXIT("accept");
        cout<<"ip="<<inet_ntoa(peeraddr.sin_addr)<<" port="<<ntohs(peeraddr.sin_port)<<endl;

        pid = fork();
        if(pid==-1) cout<<"fork"<<endl;
        if(pid==0){
            close(listenfd);
            serve(conn);
            exit(EXIT_SUCCESS);
        }
        else{
            close(conn);
        }

        

    }
    close(conn);
    close(listenfd);

    return 0;
    
    

}