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

using namespace std;

int main(){
    serve* ser = new serve();
    ser->start();

    return 0;
    
    

}