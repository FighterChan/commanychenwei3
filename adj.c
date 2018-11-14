/* 
 * adj.c
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
#include "adj.h"
#include "app.h"
#include <stdio.h>
#include <stdlib.h>

int add_adj_table(FILE *infp, struct list_head *head) {

	struct adj_table *p;
	MALLOC(struct adj_table,p);

	list_add_tail(&p->list,head);

	return APP_SUCC;
}

int free_adj_table(struct list_head *head) {

	struct list_head *pos,*next;
	struct adj_table *p;
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct adj_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}

int write_file(FILE *fp,int adj_count,struct list_head *head) {

	struct list_head *pos,*next;
	struct adj_table *p;
	fprintf(fp,"count:%d\n", adj_count);
	printf("count:%d\n", adj_count);
	/*输出*/
	list_for_each(pos,head) {
		p = list_entry(pos, struct adj_table, list);
		fprintf(fp,"%s %s %s %s %s\n", p->str_vrf, p->str_ip,
				p->str_mac, p->str_vid, p->str_interface);
	}
	return APP_SUCC;
}

