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

struct mac_table *add_mac_table(struct mac_table *s, struct list_head *head) {

	struct mac_table *p,*new_p;
	struct list_head *pos,*next;

//	ASSERT(s);
//	ASSERT(head);

#if 1
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		/*key相同，则更新已有表项*/
		if(p && strcmp(p->str_vid,s->str_vid) == 0
				&& strcmp(p->str_mac,s->str_mac) == 0) {
			strcpy(p->str_interface,s->str_interface);
			return p;
		}
	}
#endif

	MALLOC(struct mac_table,new_p);

	strcpy(new_p->str_vid,s->str_vid);
	strcpy(new_p->str_interface,s->str_interface);
	strcpy(new_p->str_mac,s->str_mac);


	list_add_tail(&new_p->list,head);

	return new_p;
}

int del_mac_table(struct mac_table *s,struct list_head *head) {

	struct list_head *pos,*next;
	struct mac_table *p;

//	ASSERT(s);
//	ASSERT(head);

	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		if ((strcmp(p->str_vid,s->str_vid) == 0) && (strcmp(p->str_mac,s->str_mac) == 0)) {
			list_del_init(pos);
			free(p);
		}
	}
	return APP_SUCC;
}

int free_mac_table(struct list_head *head) {

	struct list_head *pos,*next;
	struct mac_table *p;

//	ASSERT(head);

	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}
