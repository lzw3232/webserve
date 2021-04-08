#ifndef __TASK_H__
#define __TASK_H__

#include<vector>
#include<sys/epoll.h>

#include<mutex>
#include<condition_variable>

#include "../ERR/ERR_EXIT.h"
#include "../util/util.h"
#include "../http/http.h"

class task
{
private:
    int conn;
    int epollfd;
    std::vector<epoll_event> *events;
    int id;
    std::mutex _mutex;
    std::condition_variable cond;

public:
    task(){};
    task(int conn,int epollfd,std::vector<epoll_event> *events,int id)
        :conn(conn),epollfd(epollfd),events(events),id(id){};
    void run();
    ~task();
};



#endif