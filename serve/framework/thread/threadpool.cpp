#include "./threadpool.h"
#include<iostream>
threadpool::threadpool(int num)
{
    max_thread_num=num;
}


void threadpool::start()
{
    stop=false;
    isfull=false;
    isempty=true;
    set_num(max_thread_num);
}

void threadpool::set_num(int num)
{
    max_thread_num = num;
    for(int i=threads.size();i<max_thread_num;i++){
        threads.push_back(std::thread(&threadpool::worker,this));
    }
}


void threadpool::worker()
{
    while(!stop){
        std::unique_lock<std::mutex>lk(_mutex);
        cond.wait(lk,[this]{return !isempty;});

        task* t= tasks.front();
        //std::cout<<std::this_thread::get_id()<<std::endl;
        tasks.pop();

        if(tasks.empty()) isempty =true;
        isfull=false;
        lk.unlock(); 
        cond.notify_one();
        t->run();
        delete t;
    }
    return;
}

void threadpool::addtask(task *f)
{
    if(!stop){
        std::unique_lock<std::mutex>lk(_mutex);
        cond.wait(lk,[this]{return !isfull;});

        tasks.push(f);
       
        if(tasks.size()==max_thread_num) isfull =true;
        isempty=false;
        lk.unlock();  
        cond.notify_one();

    }
}


void threadpool::finish()
{
    stop = true;
    for(std::thread &thread: threads)
        thread.join();
    threads.clear();
    max_thread_num =0;
    stop=false;
    isfull=false;
    isempty=true;
    
}

threadpool::~threadpool()
{
    stop = true;
    for(std::thread &thread: threads)
        thread.join();
    delete ta;
}