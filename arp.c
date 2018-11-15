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

int add_arp_table(FILE *infp,struct list_head *head) {

	struct arp_table *p;
	MALLOC(struct arp_table,p);
	fscanf(infp, "%s%s%s%s", p->str_vrf,
			p->str_ip, p->str_mac,
			p->str_vid);
	list_add_tail(&p->list,head);

	return APP_SUCC;
}

int del_arp_table(FILE *infp,struct list_head *head) {

	struct arp_key {
		char vrf[32 + 1];
		char ip[16 + 1];
	}s_key = {{0},{0}};

	struct list_head *pos,*next;
	struct arp_table *p;
	fscanf(infp, "%s%s", s_key.vrf,s_key.ip);

	printf("vrf=%s,ip=%s\n",s_key.vrf,s_key.ip);

	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct arp_table,list);
		if ((strcmp(p->str_vrf,s_key.vrf) == 0) && (strcmp(p->str_ip,s_key.ip))) {
			list_del_init(pos);
			free(p);
		}
	}
	return APP_SUCC;
}

int free_arp_table(struct list_head *head) {

	struct list_head *pos,*next;
	struct arp_table *p;
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct arp_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}

