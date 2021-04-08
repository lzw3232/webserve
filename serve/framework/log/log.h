#ifndef __LOG_H__
#define __LOG_H__

#include<cstdarg>
#include<queue>
#include<iostream>
#include<fstream>
#include<mutex>


class log
{
enum FORMAT{
    DEBUG,
    INFO,
    WARN,
    ERROR,
};
private:
    log(/* args */);
    ~log();
    char* file_name;
    std::ofstream _file;
    std::queue<char*> que;

    std::mutex mut;

public:
    static log* getInstance(){
        static log instance;
        return &instance;
    }

    bool init(char* name);
    void write_log(FORMAT format,char *text);
    void log_run();


};



 

#endif