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
#include "app.h"
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

int add_mac_table(struct mac_table *s, struct list_head *head) {

	struct mac_table *p;
	struct list_head *pos,*next;

	ASSERT(s);
	ASSERT(head);

#if 1
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		if(p && strcmp(p->str_vid,s->str_vid) == 0
				&& strcmp(p->str_interface,s->str_interface) == 0
				&& strcmp(p->str_mac,s->str_mac) == 0) {
			return APP_ERR;
		}
	}
#endif

	MALLOC(struct mac_table,p);

	strcpy(p->str_vid,s->str_vid);
	strcpy(p->str_interface,s->str_interface);
	strcpy(p->str_mac,s->str_mac);
	p->counter++;

	list_add_tail(&p->list,head);

	return APP_SUCC;
}

int del_mac_table(struct mac_table *s,struct list_head *head) {

	struct list_head *pos,*next;
	struct mac_table *p;

	ASSERT(s);
	ASSERT(head);

	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		if ((strcmp(p->str_vid,s->str_vid) == 0) && (strcmp(p->str_mac,s->str_mac) == 0)) {
//			list_del_init(pos);
//			free(p);
			if(p->counter > 0) {
				p->counter--;
			}
		}
	}
	return APP_SUCC;
}

int free_mac_table(struct list_head *head) {

	struct list_head *pos,*next;
	struct mac_table *p;

	ASSERT(head);

	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}
