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

int add_adj_table(FILE *infp, struct adj_node *padj_node) {

	padj_node->node = (struct adj_table *)malloc(sizeof(struct adj_table));
	ASSERT(padj_node->node);

	list_add_tail(&padj_node->node->list,&padj_node->head);

	return APP_SUCC;
}

int free_adj_table(struct adj_table *p,struct list_head *head) {

	struct list_head *pos,*next;
	list_for_each_safe(pos,next,head) {
		p = list_entry(pos,struct adj_table,list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}

int write_file(FILE *fp,int adj_count,struct adj_table *p,struct list_head *head) {

	struct list_head *pos,*next;
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

