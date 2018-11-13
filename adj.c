/* 
 * adj.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-10
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */
#include "list.h"
#include <stdio.h>

int add_adj_table(FILE *infp, struct adj_table *padj, struct list_head *head) {

	fscanf(infp, "%s%s%s%s%s", padj->str_vrf, padj->str_ip, padj->str_mac,
			padj->str_vid,padj->str_interface);

	list_add_tail(&padj->list, head);

	return APP_SUCC;
}

