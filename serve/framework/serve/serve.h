#ifndef __SERVE_H__
#define __SERVE_H__
#include<iostream>
#include<vector>
#include<string.h>
#include<algorithm>

#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<sys/epoll.h>
#include<mutex>
#include<condition_variable>

#include "../util/util.h"
#include "../ERR/ERR_EXIT.h"
#include "../epoll/epoll.h"
#include "../thread/threadpool.h"
#include "../task/task.h"


 class serve
 {
 private:
    int listenfd;
    int epollfd;
    std::vector<epoll_event> events;

    const int EPOLLWAIT_TIME = 10000;
    threadpool *pool;

    std::mutex _mutex;
    std::condition_variable cond;

    
 public:
    serve(int port);
    void start();
    static void response(int conn,void *a,int i);
    std::vector<epoll_event> get_events() const {return events;};
    ~serve();
 };
 

 

#endif