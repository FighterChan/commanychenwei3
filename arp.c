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

int add_arp_table(struct arp_table *s, struct list_head *head) {

	struct arp_table *p;
	struct list_head *pos,*next;

	ASSERT(s);
	ASSERT(head);

#if 1
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct arp_table,list);
		/*key相同，则更新已有表项*/
		if(p && strcmp(p->str_vrf,s->str_vrf) == 0
				&& strcmp(p->str_ip,s->str_ip) == 0) {
				strcpy(p->str_mac,s->str_mac);
				strcpy(p->str_vid,s->str_vid);
			return APP_ERR;
		}
	}
#endif

	MALLOC(struct arp_table,p);

	strcpy(p->str_vrf,s->str_vrf);
	strcpy(p->str_ip,s->str_ip);
	strcpy(p->str_mac,s->str_mac);
	strcpy(p->str_vid,s->str_vid);

	list_add_tail(&p->list,head);

	return APP_SUCC;
}

int del_arp_table(struct arp_table *s,struct list_head *head) {

	struct list_head *pos,*next;
	struct arp_table *p;

	ASSERT(s);
	ASSERT(head);

	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct arp_table,list);
		if ((strcmp(p->str_vrf,s->str_vrf) == 0) && (strcmp(p->str_ip,s->str_ip) == 0)) {
			list_del_init(pos);
			free(p);
		}
	}
	return APP_SUCC;
}

int free_arp_table(struct list_head *head) {

	struct list_head *pos,*next;
	struct arp_table *p;

	ASSERT(head);

	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct arp_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}

