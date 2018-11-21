/* /adj_table/arp.h */
#ifndef _ARP_H_
#define _ARP_H_

#include <stdio.h>
#include "list.h"
#include "app.h"

struct arp_table
{
    char str_vrf[VRF_LEN_MAX];
    char str_ip[IP_LEN_MAX];
    char str_mac[MAC_LEN_MAX];
    int int_vid;
    struct hlist_node list;
};

int
add_arp_table (struct arp_table *s, struct hlist_head *head);
int
del_arp_table (struct arp_table *s, struct hlist_head *head);
int
free_arp_table (struct hlist_head *head);
u32
get_arp_key (const char *vrf, const char *ip);

/* body of arp.h */
#endif /* _ARP_H_ */
