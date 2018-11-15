/* /adj_table/adj.h */
#ifndef _ADJ_H_
#define _ADJ_H_

#include "list.h"
#include <stdio.h>

struct adj_table {
    char str_vrf[32 + 1];
    char str_ip[16 + 1];
    char str_mac[16 + 1];
    char str_vid[4 + 1];
    char str_interface[32 + 1];
    char update_flg[2];
	struct list_head list;
};

int add_adj_table(FILE *infp, struct list_head *head);

int free_adj_table(struct list_head *head);

int del_table_by_vrf(FILE *infp,FILE *outfp, int show_log,
		struct list_head *arp_head, struct list_head *adj_head);
int del_table_by_vid(FILE *infp,FILE *outfp, int show_log,
		struct list_head *mac_head, struct list_head *adj_head);

int write_file(FILE *outfp,int show_log,int adj_count,struct list_head *head);
int write_log(const char *path,struct list_head *head);

int look_up_node(int *out_count, struct list_head *sarp_head,
		struct list_head *smac_head, struct list_head *sadj_head);

/* body of adj.h */
#endif /* _ADJ_H_ */
