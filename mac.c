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

int add_mac_table(FILE *infp, struct mac_table *p,struct list_head *head) {

	fscanf(infp, "%s%s%s", p->str_vid,
			p->str_mac, p->str_interface);

	list_add_tail(&p->list,head);

	return APP_SUCC;
}

int free_mac_table(struct mac_table *p,struct list_head *head) {

	struct list_head *pos,*next;
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}
