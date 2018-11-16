/* /chenwei3/app.h */
#ifndef _APP_H_
#define _APP_H_

#include "arp.h"
#include "mac.h"
#include "adj.h"
#include "list.h"
#include <stdlib.h>

#define DEBUG 0

#define APP_SUCC (0)
#define APP_ERR  (-1)
#define CMD_MAX (9)

enum {
	CLOSE_LOG,
	OPEN_LOG
};

enum {
	ADD_ARP = 			(unsigned long)(1 << 0),
	ADD_MAC = 			(unsigned long)(1 << 1),
	DEL_ARP = 			(unsigned long)(1 << 2),
	DEL_MAC = 			(unsigned long)(1 << 3),
	DEL_VRF = 			(unsigned long)(1 << 4),
	DEL_VID = 			(unsigned long)(1 << 5),
	SHOW_ADJ_ALL = 		(unsigned long)(1 << 6),
	SHOW_ADJ = 			(unsigned long)(1 << 7),
	SHOW_LOG = 			(unsigned long)(1 << 8)
};

#define SET_FLAG(x,y)		(x |= y)
#define CHECK_FLAG(x,y)		(x & y)
#define CHECK_FLAG_ALL(x)	(x != 0)
#define CLEAR_FLAG(x,y)		(x &= ~(y))
#define CLEAR_FLAG_ALL(x)	(x &= 0)

#define ASSERT(x)\
do { \
	if (!x) { \
	printf("error in \n file---> %s\n line---> %d\n",__FILE__,__LINE__); \
	return APP_ERR; \
	} \
}while(0)

#define MALLOC(type,x) \
do { \
	x = (type *)malloc(sizeof(type)); \
	ASSERT(x); \
}while(0)

#define MALLOC_FREE(x) \
do { \
	if(x) { \
		free(x); \
	} \
}while(0)

#define THREAD_GLOB(x)          ((x)->zg)

extern int flg;

/* body of app.h */
#endif /* _APP_H_ */
