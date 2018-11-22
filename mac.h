/* /adj_table/mac.h */
#ifndef _MAC_H_
#define _MAC_H_

#include <stdio.h>
#include "app.h"
#include "list.h"
#include "jhash.h"

struct mac_table
{
    int int_vid;
    char str_mac[MAC_LEN_MAX];
    char str_interface[INTF_LEN_MAX];
    struct hlist_node list;
};

struct hlist_head mac_head[HLIST_LEN_MAX];

int
init_mac_hash (void);
struct mac_table *
look_up_mac (struct mac_table *s);
int
add_mac_table (struct mac_table *s);
int
del_mac_table (struct mac_table *s);
int
free_mac_table (void);
u32
get_mac_key (u32 vid, const char *mac);
int
copy_to_mac (struct mac_table *p, struct mac_table *s);

/* body of mac.h */
#endif /* _MAC_H_ */

