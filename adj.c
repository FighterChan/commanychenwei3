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

//int add_adj_table(struct list_head *arp_head,struct list_head *mac_head,struct list_head *adj_head) {

//	struct adj_table *p;
//	MALLOC(struct adj_table,p);
//	struct arp_table *parp;
//	MALLOC(struct arp_table,parp);
//	struct mac_table *pmac;
//	MALLOC(struct mac_table,pmac);
//
//
//	list_add_tail(&p->list,head);

//	return APP_SUCC;
//}

int del_table_by_vrf(FILE *infp,struct list_head *arp_head,struct list_head *adj_head) {

	char vrf[32 + 1];
	memset(vrf,0,sizeof(vrf));

	struct list_head *pos,*next;

	struct arp_table *parp;

	struct adj_table *padj;

	fscanf(infp, "%s", vrf);

	printf("vrf=%s\n",vrf);

	/*删除邻接表*/
	list_for_each_safe(pos,next,adj_head) {
		padj = list_entry(pos,struct adj_table,list);
		if (strcmp(padj->str_vrf,vrf) == 0) {
			list_del_init(pos);
			free(padj);
		}
	}

	/*删除arp表*/
	list_for_each_safe(pos,next,arp_head) {
		parp = list_entry(pos,struct arp_table,list);
		if (strcmp(parp->str_vrf,vrf) == 0) {
			list_del_init(pos);
			free(parp);
		}
	}

	return APP_SUCC;
}

int del_table_by_vid(FILE *infp,struct list_head *mac_head,struct list_head *adj_head) {

	char vid[4 + 1];
	memset(vid,0,sizeof(vid));

	struct list_head *pos,*next;

	struct mac_table *pmac;

	struct adj_table *padj;

	fscanf(infp, "%s", vid);

	printf("vid=%s\n",vid);

	/*删除邻接表*/
	list_for_each_safe(pos,next,adj_head) {
		padj = list_entry(pos,struct adj_table,list);
		if (strcmp(padj->str_vid,vid) == 0) {
			list_del_init(pos);
			free(padj);
		}
	}

	/*删除mac表*/
	list_for_each_safe(pos,next,mac_head) {
		pmac = list_entry(pos,struct mac_table,list);
		if (strcmp(pmac->str_vid,vid) == 0) {
			list_del_init(pos);
			free(pmac);
		}
	}

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

int write_file(const char *path,int adj_count,struct list_head *head) {

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

int look_up_node(int *out_count, struct list_head *sarp_head,
		struct list_head *smac_head, struct list_head *sadj_head) {

	int count = 0;
	struct list_head *arp_pos,*arp_next;
	struct list_head *mac_pos,*mac_next;
	struct list_head *adj_pos,*adj_next;
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
					count++;
				}
			}
		}
	*out_count = count;
	return APP_SUCC;
}

