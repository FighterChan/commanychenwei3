/* /adj_table/mac.h */
#ifndef _MAC_H_
#define _MAC_H_

#include <stdio.h>
#include "app.h"
#include "list.h"

struct mac_table
{
    int ini_vid;
    char str_mac[MAC_LEN_MAX];
    char str_interface[INTF_LEN_MAX];
    struct hlist_node list;
};

int
add_mac_table (struct mac_table *s, struct hlist_head *head);
int
del_mac_table (struct mac_table *s, struct hlist_head *head);
int
free_mac_table (struct hlist_head *head);
u32
get_mac_key (u32 vid, const char *mac);

/* body of mac.h */
#endif /* _MAC_H_ */

