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

int add_adj_table(FILE *infp, struct adj_node *padj_node) {

	padj_node->node = (struct adj_table *)malloc(sizeof(struct adj_table));
	ASSERT(padj_node->node);

	list_add_tail(&padj_node->node->list,&padj_node->head);

	return APP_SUCC;
}

