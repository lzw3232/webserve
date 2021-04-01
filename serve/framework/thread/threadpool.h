#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include<queue>
#include<vector>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<functional>

#include "../task/task.h"

class threadpool
{
private:
    task *ta;
    int max_thread_num;
    bool isfull;
    bool isempty;
    std::vector<std::thread> threads;
    std::queue<task*> tasks;
    bool stop;
    std::mutex _mutex;
    std::condition_variable cond;
public:
    threadpool(int num=4);
    void worker();
    void start();
    void set_num(int num);
    void addtask(task *f);
    void finish();
    ~threadpool();
};




#endif