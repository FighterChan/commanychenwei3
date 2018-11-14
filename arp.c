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
#include "app.h"
#include "arp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@ int add_arp_table (FILE *fp,int index,struct arp_table *arp);
 *@ function:
 */

int add_arp_table(FILE *infp, struct arp_table *p,struct list_head *head) {


	fscanf(infp, "%s%s%s%s", p->str_vrf,
			p->str_ip, p->str_mac,
			p->str_vid);

	list_add_tail(&p->list,head);

	return APP_SUCC;
}

int free_arp_table(struct arp_table *p,struct list_head *head) {

	struct list_head *pos,*next;
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct arp_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}

