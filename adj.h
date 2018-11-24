/* /adj_table/adj.h */
#ifndef _ADJ_H_
#define _ADJ_H_

#include "list.h"
#include "arp.h"
#include "mac.h"
#include <stdio.h>

struct adj_table
{
    char str_vrf[VRF_LEN_MAX];
    char str_ip[IP_LEN_MAX];
    char str_mac[MAC_LEN_MAX];
    int int_vid;
    char str_interface[INTF_LEN_MAX];
    int counter;
    struct list_head list;
};

struct list_head adj_head[HLIST_LEN_MAX];

int
init_adj_hash (void);
struct adj_table *
look_up_adj (struct adj_table *s);
int
add_adj_table (struct adj_table *s);
int
del_adj_table (struct adj_table *s);

int
free_adj_table (void);

u32
get_adj_key (const char *vrf, const char *ip);

int
copy_to_adj (struct adj_table *p, struct adj_table *s);

/* body of adj.h */
#endif /* _ADJ_H_ */
