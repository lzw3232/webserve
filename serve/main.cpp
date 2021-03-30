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

struct packet{
    int len;
    char buf[1024];
};

ssize_t readn(int fd, void *buf, size_t count){
    size_t nleft = count;
    ssize_t nread;
    char* bufp = (char*)buf;
    while(nleft>0){
        if((nread = recv(fd,bufp,nleft,0))<0){
            if(errno = EINTR)
                continue;
                return-1;
        }
        else if(nread==0){
            return nread;
        }
        bufp += nread;
        nleft -=nread;
    }
    return count;
}

ssize_t writen(int fd, void *buf, size_t count){
    size_t nleft = count;
    ssize_t nwrite;
    char* bufp = (char*)buf;
    while(nleft>0){
        if((nwrite = send(fd,bufp,nleft,0))<0){
            if(errno = EINTR)
                continue;
                return-1;
        }
        else if(nwrite==0){
            continue;
        }
        bufp += nwrite;
        nleft -=nwrite;
    }
    return count;
}


void serve(int conn){
    struct packet recvbuf;
    while(1){
        memset(&recvbuf,0,sizeof(recvbuf));
        //读数据
        int ret = readn(conn,&(recvbuf.len),4);
        if(ret==-1)
            ERR_EXIT("read");
        if(ret<4){
            cout<<"client close"<<endl;
            break;
        }
        int n = ntohl(recvbuf.len);
        ret = readn(conn,recvbuf.buf,n);
        if(ret==-1)
            ERR_EXIT("read");
        if(ret<n){
            cout<<"client close"<<endl;
            break;
        }
        
        fputs(recvbuf.buf,stdout);
        //写数据
        writen(conn,&recvbuf,n+4);
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