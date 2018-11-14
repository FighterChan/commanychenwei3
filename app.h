/* /chenwei3/app.h */
#ifndef _APP_H_
#define _APP_H_

#include "arp.h"
#include "mac.h"
#include "adj.h"
#include "list.h"

#define APP_SUCC (0)
#define APP_ERR  (-1)
#define CMD_MAX (9)

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


struct arp_node {
	struct arp_table *node;
	struct list_head *pos, *next;
	struct list_head head;
};

struct mac_node {
	struct mac_table *node;
	struct list_head *pos, *next;
	struct list_head head;
};

struct adj_node {
	struct adj_table *node;
	struct list_head *pos, *next;
	struct list_head head;
};

struct table_node {
	struct arp_node *parp;
	struct mac_node *pmac;
	struct adj_node *padj;
};

/* body of app.h */
#endif /* _APP_H_ */
