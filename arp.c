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

int add_arp_table(FILE *infp, struct arp_node *parp_node) {


	struct arp_table *p = parp_node->node;
	p = (struct arp_table *)malloc(sizeof(struct arp_table));
	ASSERT(p);

	fscanf(infp, "%s%s%s%s", p->str_vrf,
			p->str_ip, p->str_mac,
			p->str_vid);

	list_add_tail(&p->list,&parp_node->head);

//	list_for_each(parp_node->pos,&parp_node->head){
//		p = list_entry(parp_node->pos,struct arp_table,list);
//		printf("%s %s %s %s\n", p->str_vrf,
//				p->str_ip, p->str_mac,
//				p->str_vid);
//	}

	return APP_SUCC;
}

