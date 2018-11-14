/* /adj_table/mac.h */
#ifndef _MAC_H_
#define _MAC_H_

#include "app.h"
#include "list.h"
#include <stdio.h>

struct mac_table {
    char str_vid[4 + 1];
    char str_mac[16 + 1];
    char str_interface[32 + 1];
	struct list_head list;
};

int add_mac_table(FILE *infp, struct mac_node *pmac_node);

/* body of mac.h */
#endif /* _MAC_H_ */


