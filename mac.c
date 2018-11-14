/* 
 * mac.c
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
#include "mac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @int add_mac_table (FILE *fp,int index,struct mac_table *mac);
 * @fun
 *
 *
 */

#include "list.h"
#include <stdio.h>

int add_mac_table(FILE *infp, struct mac_node *pmac_node) {

	struct mac_table *p = pmac_node->node;
	p = (struct mac_table *)malloc(sizeof(struct mac_table));
	ASSERT(p);

	fscanf(infp, "%s%s%s", p->str_vid,
			p->str_mac, p->str_interface);

	list_add_tail(&p->list,&pmac_node->head);

	list_for_each(pmac_node->pos,&pmac_node->head){
		p = list_entry(pmac_node->pos,struct mac_table,list);
		printf("%s %s %s\n", p->str_vid,
				p->str_mac, p->str_interface);
	}

	return APP_SUCC;
}
