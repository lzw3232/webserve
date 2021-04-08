#ifndef __HTTP_H__
#define __HTTP_H__
#include<string.h>

class http
{
private:
    enum METHOD{
        GET,
        POST,
    };
    enum CHECK_STATE{
        CHECK_STATE_REQUESTLINE,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT,
    };
    enum HTTP_CODE{
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
    };
    int lisenfd;
    char* m_url;
    int cgi;        //是否启用的POST
    char *m_version;
    METHOD m_method;
    CHECK_STATE m_check_state;
public:
    http();
    ~http();
    HTTP_CODE parse_request_line(char *text);
    char* do_request(char *text);
};




 

 

#endif