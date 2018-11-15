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
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int write_file(const char  *path,int adj_count,struct list_head *head) {

	struct list_head *pos,*next;
	struct adj_table *p;
	char outpath[32];
	memset(outpath,0,sizeof(outpath));
	conver_filename(path,outpath);
	FILE *outfp;
	outfp = fopen(outpath, "w");
	ASSERT(outfp);
	fprintf(outfp,"count:%d\n", adj_count);
	printf("count:%d\n", adj_count);
	/*输出*/
	list_for_each(pos,head) {
		p = list_entry(pos, struct adj_table, list);
		fprintf(outfp,"%s %s %s %s %s\n", p->str_vrf, p->str_ip,
				p->str_mac, p->str_vid, p->str_interface);
	}
	fclose(outfp);
	return APP_SUCC;
}

int look_up_node(const char *path, struct list_head *sarp_head,
		struct list_head *smac_head, struct list_head *sadj_head) {

	int count = 0;
	struct list_head *arp_pos;
	struct list_head *mac_pos;
	struct list_head *adj_pos;
	struct arp_table *parp_t;
	struct mac_table *pmac_t;
	struct adj_table *padj_t;

	list_for_each(arp_pos,sarp_head) {
		parp_t = list_entry(arp_pos, struct arp_table, list);
		list_for_each(mac_pos,smac_head) {
			pmac_t = list_entry(mac_pos, struct mac_table, list);
			if ((strcmp(parp_t->str_mac, pmac_t->str_mac) == 0
					&& strcmp(parp_t->str_vid, pmac_t->str_vid) == 0)) {
				MALLOC(struct adj_table,padj_t);
				strcpy(padj_t->str_vrf, parp_t->str_vrf);
				strcpy(padj_t->str_ip, parp_t->str_ip);
				strcpy(padj_t->str_mac, parp_t->str_mac);
				strcpy(padj_t->str_vid, parp_t->str_vid);
				strcpy(padj_t->str_interface, pmac_t->str_interface);
				list_add_tail(&padj_t->list, sadj_head);
//				printf("%s %s %s %s %s\n", padj_t->str_vrf, padj_t->str_ip,
//								padj_t->str_mac, padj_t->str_vid, padj_t->str_interface);
				count++;
			}
		}
	}

	/* 输出adj到文件 */
	write_file(path,count,sadj_head);

	return APP_SUCC;
}

