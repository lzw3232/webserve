#include<iostream>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include<string.h>


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

int main(){
    int listenfd;

    if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0)
            ERR_EXIT("socket");
    
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        cout<<"bind"<<endl;

    struct packet sendbuf;
    struct packet recvbuf;
    memset(&recvbuf,0,sizeof(recvbuf));
    memset(&sendbuf,0,sizeof(sendbuf));
    int len;
    while(fgets(sendbuf.buf,sizeof(sendbuf.buf),stdin)!=NULL){
        len = strlen(sendbuf.buf);
        sendbuf.len = ntohl(len);
        //写数据
        writen(listenfd,&sendbuf,len+4);
        //读数据
        int ret = readn(listenfd,&(recvbuf.len),4);
        if(ret==-1)
            ERR_EXIT("read");
        if(ret<4){
            cout<<"client close"<<endl;
            break;
        }
        int n = ntohl(recvbuf.len);
        ret = readn(listenfd,recvbuf.buf,n);
        if(ret==-1)
            ERR_EXIT("read");
        if(ret<n){
            cout<<"client close"<<endl;
            break;
        }
        fputs(sendbuf.buf,stdout);

        memset(&recvbuf,0,sizeof(recvbuf));
        memset(&sendbuf,0,sizeof(sendbuf));

    }
    close(listenfd);

    return 0;
    
    

}