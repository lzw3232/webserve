#include "./util.h"

int MAX_BUF = 4096;

ssize_t readn(int fd, void *buf, size_t count){
    size_t nleft = count;
    ssize_t nread=0;
    ssize_t readNum=0;
    char* bufp = (char*)buf;
    while(nleft>0){
        if((nread = recv(fd,bufp,nleft,0))<0){
            if(errno = EINTR)
                continue;
                return-1;
        }
        else if(nread==0){
            return readNum;
        }
        bufp += nread;
        nleft -=nread;
        readNum+=nread;
    }
    return readNum;
}


ssize_t writen(int fd, void *buf, size_t count){
    size_t nleft = count;
    ssize_t nwrite;
    ssize_t writeNum;
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
        writeNum+=nwrite;
    }
    return writeNum;
}

int setSocketNonBlocking(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1)
        return -1;

    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1)
        return -1;
    return 0;
}
