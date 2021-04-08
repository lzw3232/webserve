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

#include "./framework/util/util.h"
#include "./framework/ERR/ERR_EXIT.h"
#include "./framework/serve/serve.h"
#include "./framework/thread/threadpool.h"


using namespace std;

int main(){
    int port = 5188;
    serve* ser = new serve(port);
    ser->start();
    // threadpool *p = new threadpool(5);
    // p->start();
    // for(int i=0;i<100;i++){
    //     auto task = bind(m,i,1);
    //     p->addtask(task);
    //     if(i==10) p->set_num(8);
    // }
    // sleep(4);
    // p->finish();
    // delete p;

    return 0;
    
    

}