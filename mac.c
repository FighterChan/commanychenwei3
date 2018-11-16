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

int add_mac_table(FILE *infp, struct list_head *head) {

	struct mac_table *p;
	struct mac_table s = {{0},{0},{0},NULL};
	struct list_head *pos;

	fscanf(infp, "%s%s%s", s.str_vid,
			s.str_mac, s.str_interface);
	list_for_each(pos,head) {
		p = list_entry(pos,struct mac_table,list);
		if(strcmp(p->str_vid,s.str_vid) == 0
				&& strcmp(p->str_interface,s.str_interface) == 0
				&& strcmp(p->str_mac,s.str_mac) == 0) {
			return -1;
		}
	}

	MALLOC(struct mac_table,p);

	strcpy(p->str_vid,s.str_vid);
	strcpy(p->str_interface,s.str_interface);
	strcpy(p->str_mac,s.str_mac);


	list_add_tail(&p->list,head);

	return APP_SUCC;
}

int del_mac_table(FILE *infp,struct list_head *head) {

	struct mac_key {
		char vid[4 + 1];
		char mac[16 + 1];
	}s_key = {{0},{0}};

	struct list_head *pos,*next;
	struct mac_table *p;
	fscanf(infp, "%s%s", s_key.vid,s_key.mac);

	printf("vid=%s,mac=%s\n",s_key.vid,s_key.mac);

	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		if ((strcmp(p->str_vid,s_key.vid) == 0) && (strcmp(p->str_mac,s_key.mac) == 0)) {
			list_del_init(pos);
			free(p);
		}
	}
	return APP_SUCC;
}

int free_mac_table(struct list_head *head) {

	struct list_head *pos,*next;
	struct mac_table *p;
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct mac_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}
