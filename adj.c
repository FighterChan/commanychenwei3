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
#include "mac.h"
#include "app.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_adj_table(FILE *fp, struct adj_table *s, struct list_head *head) {

	struct adj_table *p;
	struct list_head *pos, *next;

	ASSERT(s);
	ASSERT(head);

#if 1
	list_for_each_safe(pos,next,head)
	{
		p = list_entry(pos, struct adj_table, list);
		if (p && strcmp(p->str_vid, s->str_vid) == 0
				&& strcmp(p->str_interface, s->str_interface) == 0
				&& strcmp(p->str_mac, s->str_mac) == 0
				&& strcmp(p->str_ip, s->str_ip) == 0
				&& strcmp(p->str_vrf, s->str_vrf) == 0) {
			return APP_ERR;
		}
	}
#endif

	MALLOC(struct adj_table, p);

	strcpy(p->str_vid, s->str_vid);
	strcpy(p->str_interface, s->str_interface);
	strcpy(p->str_mac, s->str_mac);
	strcpy(p->str_ip, s->str_ip);
	strcpy(p->str_vrf, s->str_vrf);
	p->counter++;

	if (CHECK_FLAG(flg,SHOW_LOG) != 0) {
		fprintf(fp, "%s %s %s %s %s %s\n", "add-adj", p->str_vrf, p->str_ip,
				p->str_mac, p->str_vid, p->str_interface);
	}

	list_add_tail(&p->list, head);

	return APP_SUCC;
}

int del_adj_table(FILE *fp, struct adj_table *s, struct list_head *head) {

	struct list_head *pos, *next;
	struct adj_table *p;

	ASSERT(fp);
	ASSERT(s);
	ASSERT(head);

	list_for_each_safe(pos,next,head)
	{
		p = list_entry(pos, struct adj_table, list);
		if (p->counter > 0) {
			p->counter--;
		}
		if (CHECK_FLAG(flg,SHOW_LOG) != 0) {
			fprintf(fp, "%s %s %s %s %s %s\n", "del-adj", p->str_vrf, p->str_ip,
					p->str_mac, p->str_vid, p->str_interface);
		}
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}

int del_table_by_vrf(FILE *fp, struct arp_table *s, struct list_head *arp_head,
		struct list_head *adj_head) {

	struct list_head *pos, *next;

	struct arp_table *parp;

	struct adj_table *padj;

	ASSERT(fp);
	ASSERT(s);
	ASSERT(arp_head);
	ASSERT(adj_head);
#if 0
	/*删除邻接表*/
	list_for_each_safe(pos,next,adj_head)
	{
		padj = list_entry(pos, struct adj_table, list);
		if (strcmp(padj->str_vrf, s->str_vrf) == 0) {
			/*标志老化节点*/
			if (padj->counter > 0) {
				padj->counter--;
			}
			if (CHECK_FLAG(flg,SHOW_LOG) != 0) {
				fprintf(fp, "%s %s %s %s %s %s\n", "del-adj", padj->str_vrf,
						padj->str_ip, padj->str_mac, padj->str_vid,
						padj->str_interface);
			}
//			list_del_init(pos);
//			free(padj);
		}
	}
#endif

	/*删除arp表*/
	list_for_each_safe(pos,next,arp_head)
	{
		parp = list_entry(pos, struct arp_table, list);
		if (strcmp(parp->str_vrf, s->str_vrf) == 0) {
//			list_del_init(pos);
//			free(parp);
			if (parp->counter > 0) {
				parp->counter--;
			}
		}
	}
	return APP_SUCC;
}

int del_table_by_vid(FILE *fp, struct mac_table *s, struct list_head *mac_head,
		struct list_head *adj_head) {

	struct list_head *pos, *next;

	struct mac_table *pmac;

	struct adj_table *padj;

	ASSERT(fp);
	ASSERT(s);
	ASSERT(mac_head);
	ASSERT(adj_head);
#if 0
	/*删除邻接表*/
	list_for_each_safe(pos,next,adj_head)
	{
		padj = list_entry(pos, struct adj_table, list);
		if (strcmp(padj->str_vid, s->str_vid) == 0) {
			/*标志老化节点*/
			if (padj->counter > 0) {
				padj->counter--;
			}
			if (CHECK_FLAG(flg,SHOW_LOG) != 0) {
				fprintf(fp, "%s %s %s %s %s %s\n", "del-adj", padj->str_vrf,
						padj->str_ip, padj->str_mac, padj->str_vid,
						padj->str_interface);
//				list_del_init(pos);
//				free(padj);
			}
		}
	}
#endif
	/*删除mac表*/
	list_for_each_safe(pos,next,mac_head)
	{
		pmac = list_entry(pos, struct mac_table, list);
		if (strcmp(pmac->str_vid, s->str_vid) == 0) {
//			list_del_init(pos);
//			free(pmac);
			if (pmac->counter > 0) {
				pmac->counter--;
			}
		}
	}

	return APP_SUCC;
}

int free_adj_table(struct list_head *head) {

	struct list_head *pos, *next;
	struct adj_table *p;

	ASSERT(head);

	list_for_each_safe(pos,next,head)
	{
		p = list_entry(pos, struct adj_table, list);
		list_del_init(pos);
		free(p);
	}
	return APP_SUCC;
}

int write_file(FILE *outfp, struct list_head *head) {

	struct list_head *pos, *next;
	struct adj_table *p;
	int index;

	index = 0;
	list_for_each(pos,head)
	{
		index++;
	}

	fprintf(outfp, "count:%d\n", index);

	list_for_each_safe(pos,next,head)
	{
		p = list_entry(pos, struct adj_table, list);
		fprintf(outfp, "%s %s %s %s %s %s\n", "add-adj", p->str_vrf, p->str_ip,
				p->str_mac, p->str_vid, p->str_interface);
	}
	return APP_SUCC;
}

int look_up_node(FILE *fp, struct list_head *sarp_head,
		struct list_head *smac_head, struct list_head *sadj_head) {

	struct list_head *arp_pos, *arp_next;
	struct list_head *mac_pos, *mac_next;
	struct arp_table *parp;
	struct mac_table *pmac;
	struct adj_table sadj;

	ASSERT(fp);
	ASSERT(sarp_head);
	ASSERT(smac_head);
	ASSERT(sadj_head);

	memset(&sadj, 0, sizeof(struct adj_table));

	list_for_each_safe(arp_pos,arp_next,sarp_head)
	{
		if (parp->counter > 0) {
			parp = list_entry(arp_pos, struct arp_table, list);
			list_for_each_safe(mac_pos,mac_next,smac_head)
			{
				if (pmac->counter > 0) {
					pmac = list_entry(mac_pos, struct mac_table, list);
					if ((strcmp(parp->str_mac, pmac->str_mac) == 0
							&& strcmp(parp->str_vid, pmac->str_vid) == 0)) {
						strcpy(sadj.str_interface, pmac->str_interface);
						strcpy(sadj.str_ip, parp->str_ip);
						strcpy(sadj.str_vrf, parp->str_vrf);
						strcpy(sadj.str_mac, parp->str_mac);
						strcpy(sadj.str_vid, parp->str_vid);

						printf("parp->counter=%d,pmac->coutner=%d\n",
								parp->counter, pmac->counter);
						if (parp->counter > 0 && pmac->counter > 0) {
							add_adj_table(fp, &sadj, sadj_head);
						} else {
							del_adj_table(fp, &sadj, sadj_head);
						}
					}
				} else {
					list_del_init(mac_pos);
					free(pmac);
				}
			}
		} else {
			list_del_init(arp_pos);
			free(parp);
		}
	}
	return APP_SUCC;
}

