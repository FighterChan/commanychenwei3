/* 
 * arp.c
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
#include "arp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@ int add_arp_table (FILE *fp,int index,struct arp_table *arp);
 *@ function:
 */

int add_arp_table(FILE *infp, struct table_node *pnode) {

	pnode->parp = (struct arp_node *) malloc(sizeof(struct arp_node));
	ASSERT(pnode->parp);

	memset(&pnode->parp->head, 0, sizeof(struct list_head));

	INIT_LIST_HEAD(&pnode->parp->head);

	pnode->parp->node = (struct arp_table *)malloc(sizeof(struct arp_table));
	ASSERT(pnode->parp->node);

	fscanf(infp, "%s%s%s%s", pnode->parp->node->str_vrf,
			pnode->parp->node->str_ip, pnode->parp->node->str_mac,
			pnode->parp->node->str_vid);

	list_add_tail(&pnode->parp->node->list,&pnode->parp->head);

	list_for_each(pnode->parp->pos,&pnode->parp->head){
		pnode->parp->node = list_entry(pnode->parp->pos,struct arp_table,list);
		printf("%s %s %s %s\n", pnode->parp->node->str_vrf,
				pnode->parp->node->str_ip, pnode->parp->node->str_mac,
				pnode->parp->node->str_vid);
	}

	return APP_SUCC;
}

