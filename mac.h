/* /adj_table/mac.h */
#ifndef _MAC_H_
#define _MAC_H_

#include "list.h"
#include <stdio.h>

struct mac_table {
    int int_vid;
    char str_mac[16 + 1];
    char str_interface[32 + 1];
	struct list_head list;
};

int add_mac_table(FILE *infp,struct list_head *head);
int del_mac_table(FILE *infp,struct list_head *head);
int free_mac_table(struct list_head *head);

/* body of mac.h */
#endif /* _MAC_H_ */


