/* /adj_table/arp.h */
#ifndef _ARP_H_
#define _ARP_H_

#include <stdio.h>
#include "list.h"
#include "app.h"
#include "jhash.h"

struct arp_table
{
    char str_vrf[VRF_LEN_MAX];
    char str_ip[IP_LEN_MAX];
    char str_mac[MAC_LEN_MAX];
    int int_vid;
    struct list_head list;
};

struct list_head arp_head[HLIST_LEN_MAX];

int
init_arp_hash (void);
struct arp_table *
look_up_arp (const char *vrf,const char *ip);
int
add_arp_table (struct arp_table *s);
int
del_arp_table (const char *vrf, const char *ip);
int
free_arp_table (void);
u32
get_arp_key (const char *vrf, const char *ip);
int
copy_to_arp ( struct arp_table *p,struct arp_table *s);

/* body of arp.h */
#endif /* _ARP_H_ */
