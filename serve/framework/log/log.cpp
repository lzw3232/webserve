#include "./log.h"

log::log(/* args */)
{
}

log::~log()
{
}

bool log::init(char* name){
    file_name = name;
    _file.open(file_name);
    if(!_file)
    {   
        std::cout<<"创建log失败"<<std::endl;
        return false;
    }
    log_run();
}

void log::write_log(FORMAT format,char* text){

}

void log::log_run(){

}