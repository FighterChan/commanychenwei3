/* /adj_table/adj.h */
#ifndef _ADJ_H_
#define _ADJ_H_

#include "list.h"
#include "arp.h"
#include "mac.h"
#include <stdio.h>

struct adj_table {
    char str_vrf[32 + 1];
    char str_ip[16 + 1];
    char str_mac[16 + 1];
    char str_vid[4 + 1];
    char str_interface[32 + 1];
    int counter;
    struct list_head list;
};

int
add_adj_table(FILE *fp, struct adj_table *s, struct list_head *head);
int
del_adj_table(FILE *fp, struct list_head *head);

int
free_adj_table(struct list_head *head);

int
del_table_by_vrf(FILE *fp, struct arp_table *s, struct list_head *arp_head,
        struct list_head *adj_head);
int
del_table_by_vid(FILE *fp, struct mac_table *s, struct list_head *mac_head,
        struct list_head *adj_head);

int
write_file(FILE *outfp, char *vrf, struct list_head *head);

int
update_daj_node(FILE *fp, struct list_head *sarp_head,
        struct list_head *smac_head, struct list_head *sadj_head);

/* body of adj.h */
#endif /* _ADJ_H_ */
