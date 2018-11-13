/* /adj_table/main.h */
#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include "arp.h"
#include "mac.h"
#include "adj.h"
#include "list.h"

enum {
	FALSE, TRUE = !FALSE
};

enum {
	SHOW_SELECT, SHOW_ALL,
};

#define APP_SUCC (0)
#define APP_ERR  (-1)
#define CMD_MAX (9)


#define DEFINE_PARAM(x)\
do { \
	if (!x) { \
	printf("%s error!\n",x); \
	return APP_ERR; \
	} \
}while(0)


struct table_node {
	struct arp_table *parp;
	struct mac_table *pmac;
	struct adj_table *padj;
	struct list_head *arp_pos, *arp_next;
	struct list_head *mac_pos, *mac_next;
	struct list_head *adj_pos, *adj_next;
};

/* body of main.h */
#endif /* _MAIN_H_ */
