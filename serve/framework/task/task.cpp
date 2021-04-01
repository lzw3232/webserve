#include "./task.h"
#include<iostream>

void task::run(){
    char recvbuf[1024];
    struct epoll_event event;
    int ret = readn(conn,recvbuf,sizeof(recvbuf));
    if(ret==-1)
        ERR_EXIT("read");
    if(ret=0){
        std::cout<<"client close"<<std::endl;
        close(conn);
        event = (*events)[id];

        std::unique_lock<std::mutex>lk(_mutex);
        epoll_ctl(epollfd,EPOLL_CTL_DEL,conn,&event);
        events->erase(events->begin()+id,events->begin()+id+1);
        lk.unlock();
    
    }
    else{
        fputs(recvbuf,stdout);
        //写数据
        writen(conn,&recvbuf,sizeof(recvbuf));
    }
    return;
}

task::~task()
{
}