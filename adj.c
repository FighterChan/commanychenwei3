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

int del_table_by_vrf(FILE *infp, FILE *outfp, int show_log,
		struct list_head *arp_head, struct list_head *adj_head) {

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
//			printf("file %s,line %d,show_log = %d\n",__FILE__,__LINE__,show_log);
			if (show_log == CLOSE_LOG) {

			} else {
				fprintf(outfp, "%s %s %s %s %s %s\n", "del-adj", padj->str_vrf,
						padj->str_ip, padj->str_mac, padj->str_vid, padj->str_interface);

			}
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

int del_table_by_vid(FILE *infp, FILE *outfp, int show_log,
		struct list_head *mac_head, struct list_head *adj_head) {


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
//			printf("file %s,line %d,show_log = %d\n",__FILE__,__LINE__,show_log);
			if (show_log == CLOSE_LOG) {

			} else {
				fprintf(outfp, "%s %s %s %s %s %s\n", "del-adj", padj->str_vrf,
						padj->str_ip, padj->str_mac, padj->str_vid, padj->str_interface);
			}
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

int write_file(FILE *outfp,int show_log,int adj_count,struct list_head *head) {

	struct list_head *pos,*next;
	struct adj_table *p;
	if (show_log == CLOSE_LOG
			&& (CHECK_FLAG(flg,SHOW_ADJ_ALL) != 0
					|| CHECK_FLAG(flg,SHOW_ADJ) != 0)) {
		fprintf(outfp,"count:%d\n", adj_count);
//	    printf("count:%d\n", adj_count);
	}
	/*输出*/
	int i = 0;
	list_for_each(pos,head) {
		p = list_entry(pos, struct adj_table, list);
//		printf("file %s,line %d,show_log = %d\n",__FILE__,__LINE__,show_log);
		if (show_log == CLOSE_LOG
				&& (CHECK_FLAG(flg,SHOW_ADJ_ALL) != 0
						|| CHECK_FLAG(flg,SHOW_ADJ) != 0)) {
			fprintf(outfp, "%s %s %s %s %s\n", p->str_vrf, p->str_ip,
					p->str_mac, p->str_vid, p->str_interface);
		} else {
			fprintf(outfp, "%s %s %s %s %s %s\n", "add-adj", p->str_vrf,
					p->str_ip, p->str_mac, p->str_vid, p->str_interface);
		}
		printf("i= %d\n",i++);
	}
	return APP_SUCC;
}
#if 0
int write_log(const char *path,struct list_head *head) {

	char outpath[32];
	memset(outpath, 0, sizeof(outpath));

	struct list_head *pos, *next;
	struct adj_table *p;

	conver_filename(path, outpath);
	FILE *outfp;
	outfp = fopen(outpath, "w");
	ASSERT(outfp);

	list_for_each(pos,head) {
		p = list_entry(pos, struct adj_table, list);
		if (strcmp(p->update_flg, "01") == 0) {
			fprintf(outfp, "%s %s %s %s %s %s\n", "add_adj", p->str_vrf, p->str_ip,
					p->str_mac, p->str_vid, p->str_interface);
		} else if (strcmp(p->update_flg, "02") == 0) {
			fprintf(outfp, "%s %s %s %s %s %s\n", "del_adj", p->str_vrf, p->str_ip,
					p->str_mac, p->str_vid, p->str_interface);
		}
	}

	return APP_SUCC;
}
#endif

int look_up_node(int *out_count,struct list_head *sarp_head,
		struct list_head *smac_head, struct list_head *sadj_head) {

	int count = 0;
	struct list_head *arp_pos, *arp_next;
	struct list_head *mac_pos, *mac_next;
	struct list_head *adj_pos, *adj_next;
	struct arp_table *parp_t;
	struct mac_table *pmac_t;
	struct adj_table *padj_t;

	list_for_each_safe(arp_pos,arp_next,sarp_head)
	{
		parp_t = list_entry(arp_pos, struct arp_table, list);
		list_for_each_safe(mac_pos,mac_next,smac_head)
		{
			pmac_t = list_entry(mac_pos, struct mac_table, list);
			if ((strcmp(parp_t->str_mac, pmac_t->str_mac) == 0
					&& strcmp(parp_t->str_vid, pmac_t->str_vid) == 0)) {

//				list_for_each(adj_pos,sadj_head) {
//					padj_t = list_entry(adj_pos,struct adj_table,list);
//					if (padj_t && strcmp(padj_t->str_vrf, parp_t->str_vrf) == 0
//							&& strcmp(padj_t->str_ip, parp_t->str_ip) == 0
//							&& strcmp(padj_t->str_mac, parp_t->str_mac) == 0
//							&& strcmp(padj_t->str_vid, parp_t->str_vid)
//							&& strcmp(padj_t->str_interface, pmac_t->str_interface)
//									== 0) {
//						return -1;
//					}
//				}
				MALLOC(struct adj_table, padj_t);
				strcpy(padj_t->str_vrf, parp_t->str_vrf);
				strcpy(padj_t->str_ip, parp_t->str_ip);
				strcpy(padj_t->str_mac, parp_t->str_mac);
				strcpy(padj_t->str_vid, parp_t->str_vid);
				strcpy(padj_t->str_interface, pmac_t->str_interface);
				/*更新*/
				list_add_tail(&padj_t->list, sadj_head);
				count++;
			}
		}
	}
	*out_count = count;
	return APP_SUCC;
}

