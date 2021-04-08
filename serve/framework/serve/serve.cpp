#include "./serve.h"

serve::serve(int port)
{
    //启动日志
    if(!log::getInstance()->init("web.log"))
        ERR_EXIT("log");

    //绑定post
    if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0)
        ERR_EXIT("socket");
    
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int on = 1;
	setsockopt(listenfd , SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) ;

    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        ERR_EXIT("bind");

    if(listen(listenfd,SOMAXCONN)<0)
        ERR_EXIT("listen");

    pool = new threadpool(4);
    pool->start();
}

void serve::start(){
    //初始化客户端的地址
    struct sockaddr_in peeraddr;
    memset(&peeraddr,0,sizeof(peeraddr));
    socklen_t peerlen = sizeof(peeraddr);

    int conn;

    epollfd = epoll_create1(EPOLL_CLOEXEC);
    struct epoll_event event;
    event.data.fd = listenfd;
    event.events = EPOLLIN | EPOLLET|EPOLLRDHUP;
    events.push_back(event);
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&event);

    while(1){
        int nread = epoll_wait(epollfd,&*events.begin(),events.size(),EPOLLWAIT_TIME);
        if(nread<0) continue;
        if(nread==0) continue;
        for(int i=0;i<nread;i++){
            if(events[i].data.fd==listenfd){
                if((conn = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
                    ERR_EXIT("accept");
                std::cout<<"ip="<<inet_ntoa(peeraddr.sin_addr)<<" port="<<ntohs(peeraddr.sin_port)<<std::endl;

                setSocketNonBlocking(conn);

                event.data.fd = conn;
                event.events = EPOLLIN | EPOLLET|EPOLLRDHUP;
                events.push_back(event);
                epoll_ctl(epollfd,EPOLL_CTL_ADD,conn,&event);
            }
            else if(events[i].events & EPOLLRDHUP){
                conn = events[i].data.fd;
                std::cout<<"client close"<<std::endl;
                close(conn);
                event = events[i];
                epoll_ctl(epollfd,EPOLL_CTL_DEL,conn,&event);
                events.erase(events.begin()+i,events.begin()+i+1);

            }
            else if(events[i].events & EPOLLIN){
                conn = events[i].data.fd;
                if(conn<0) ERR_EXIT("conn");
                // response(conn,i);

                task* ta = new task(conn,epollfd,&events,i);
                pool->addtask(ta);

                // char recvbuf[1024];
                // struct epoll_event event;
                // int ret = readn(conn,recvbuf,sizeof(recvbuf));
                // if(ret==-1)
                //     ERR_EXIT("read");
                // if(ret=0){
                //     std::cout<<"client close"<<std::endl;
                //     close(conn);
                //     event = events[i];

                //     std::unique_lock<std::mutex>lk(_mutex);
                //     epoll_ctl(epollfd,EPOLL_CTL_DEL,conn,&event);
                //     events.erase(events.begin()+i,events.begin()+i+1);
                //     lk.unlock();
                
                // }
                // else{
                //     fputs(recvbuf,stdout);
                //     //写数据
                //     writen(conn,&recvbuf,sizeof(recvbuf));
                // }
            }

        }
    }
    close(conn);
    close(listenfd);
}


// static void serve::response(int conn,void *a,int i)
// {
//     char recvbuf[1024];
//     struct epoll_event event;
//     int ret = readn(conn,recvbuf,sizeof(recvbuf));
//     if(ret==-1)
//         ERR_EXIT("read");
//     if(ret=0){
//         std::cout<<"client close"<<std::endl;
//         close(conn);
//         event = events[i];

//         std::unique_lock<std::mutex>lk(_mutex);
//         epoll_ctl(epollfd,EPOLL_CTL_DEL,conn,&event);
//         events.erase(events.begin()+i,events.begin()+i+1);
//         lk.unlock();
    
//     }
//     else{
//         fputs(recvbuf,stdout);
//         //写数据
//         writen(conn,&recvbuf,sizeof(recvbuf));
//     }
//     return;
// }

serve::~serve()
{
    delete pool;
}