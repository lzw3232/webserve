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

#include "../util/util.h"
#include "../ERR/ERR_EXIT.h"
#include "../epoll/epoll.h"


 class serve
 {
 private:
    int listenfd;
    const int EPOLLWAIT_TIME = 10000;
 public:
     serve();
     void start();
     ~serve();
 };
 

 

#endif