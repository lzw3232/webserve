#ifndef __UTIL_H__
#define __UTIL_H__
#include<iostream>
#include<unistd.h>
#include<netinet/in.h>
#include <fcntl.h>


ssize_t readn(int fd, void *buf, size_t count);
ssize_t writen(int fd, void *buf, size_t count);
int setSocketNonBlocking(int fd);

#endif