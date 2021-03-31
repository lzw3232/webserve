#define ERR_EXIT(m) \
     do \
	 { \
		perror(m); \
		exit(EXIT_FAILURE);\
	 }while(0)