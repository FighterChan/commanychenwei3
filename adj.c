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

struct adj_table *add_adj_table(FILE *fp, struct adj_table *s, struct list_head *head) {

	struct adj_table *p,*new_p;
	struct list_head *pos, *next;

//	ASSERT(s);
//	ASSERT(head);

#if 1
	list_for_each_safe(pos,next,head)
	{
		p = list_entry(pos, struct adj_table, list);
		if (p && strcmp(p->str_ip, s->str_ip) == 0
				&& strcmp(p->str_vrf, s->str_vrf) == 0
				&& strcmp(p->str_interface,s->str_interface) == 0
				&& strcmp(p->str_mac,s->str_mac) == 0
				&& strcmp(p->str_vid,s->str_vid) == 0) {

			return p;
		}
	}
#endif

	MALLOC(struct adj_table, new_p);

	strcpy(new_p->str_vid, s->str_vid);
	strcpy(new_p->str_interface, s->str_interface);
	strcpy(new_p->str_mac, s->str_mac);
	strcpy(new_p->str_ip, s->str_ip);
	strcpy(new_p->str_vrf, s->str_vrf);
	new_p->counter = 0;
	if (CHECK_FLAG(flg,SHOW_LOG) != 0) {
		fprintf(fp, "%s %s %s %s %s %s\n", "add-adj", new_p->str_vrf, new_p->str_ip,
				new_p->str_mac, new_p->str_vid, new_p->str_interface);
	}
	list_add_tail(&new_p->list, head);

	return new_p;
}

int del_adj_table(FILE *fp, struct list_head *head) {

	struct adj_table *p;
	struct list_head *pos, *next;

	list_for_each_safe(pos,next,head)
	{
		p = list_entry(pos, struct adj_table, list);
		if (p->counter == 1) {
			if (CHECK_FLAG(flg,SHOW_LOG) != 0) {
				fprintf(fp, "%s %s %s %s %s %s\n", "del-adj", p->str_vrf,
						p->str_ip, p->str_mac, p->str_vid, p->str_interface);
			}
			list_del_init(pos);
			free(p);
		}
	}

	return APP_SUCC;
}

int del_table_by_vrf(FILE *fp, struct arp_table *s, struct list_head *arp_head,
		struct list_head *adj_head) {

	struct list_head *pos, *next;

	struct arp_table *parp;

	struct adj_table *padj;
#if 0
	/*删除邻接表*/
	list_for_each_safe(pos,next,adj_head)
	{
		padj = list_entry(pos, struct adj_table, list);
		if (strcmp(padj->str_vrf, s->str_vrf) == 0) {
			/*标志老化节点*/
			padj->counter--;
			if (CHECK_FLAG(flg,SHOW_LOG) != 0) {
				fprintf(fp, "%s %s %s %s %s %s\n", "del-adj", padj->str_vrf,
						padj->str_ip, padj->str_mac, padj->str_vid,
						padj->str_interface);
			}
			list_del_init(pos);
			free(padj);
		}
	}
#endif
	/*删除arp表*/
	list_for_each_safe(pos,next,arp_head)
	{
		parp = list_entry(pos, struct arp_table, list);
		if (strcmp(parp->str_vrf, s->str_vrf) == 0) {
			list_del_init(pos);
			free(parp);
		}
	}
	return APP_SUCC;
}

int del_table_by_vid(FILE *fp, struct mac_table *s, struct list_head *mac_head,
		struct list_head *adj_head) {

	struct list_head *pos, *next;

	struct mac_table *pmac;

	struct adj_table *padj;
#if 0
	/*删除邻接表*/
	list_for_each_safe(pos,next,adj_head)
	{
		padj = list_entry(pos, struct adj_table, list);
		if (strcmp(padj->str_vid, s->str_vid) == 0) {
			/*标志老化节点*/
			padj->counter--;
			if (CHECK_FLAG(flg,SHOW_LOG) != 0) {
				fprintf(fp, "%s %s %s %s %s %s\n", "del-adj", padj->str_vrf,
						padj->str_ip, padj->str_mac, padj->str_vid,
						padj->str_interface);
			}
			list_del_init(pos);
			free(padj);
		}
	}
#endif
	/*删除mac表*/
	list_for_each_safe(pos,next,mac_head)
	{
		pmac = list_entry(pos, struct mac_table, list);
		if (strcmp(pmac->str_vid, s->str_vid) == 0) {
			list_del_init(pos);
			free(pmac);
		}
	}

	return APP_SUCC;
}

int free_adj_table(struct list_head *head) {

	struct list_head *pos, *next;
	struct adj_table *p;

//	ASSERT(head);

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
		fprintf(outfp, "%s %s %s %s %s\n", p->str_vrf, p->str_ip, p->str_mac,
				p->str_vid, p->str_interface);
	}
	return APP_SUCC;
}

/*
 *adj_lookup：查询adj表项
 */
struct adj_table *adj_lookup(const char *vrf, const char *ip,
    const char *mac, const char *vid, const char *intf,struct list_head *head)
{
    struct adj_table *padj;
    struct list_head *pos, *next;

    list_for_each_safe(pos, next, head) {
        padj = list_entry(pos, struct adj_table, list);

        if (!strcmp(padj->str_vrf, vrf) && !strcmp(padj->str_ip, ip) && !strcmp(padj->str_mac, mac) &&
                (!strcmp(padj->str_vid,vid)) && !strcmp(padj->str_interface, intf)) {

            return padj;
        }
    }

    return NULL;
}

int look_up_node(FILE *fp, struct list_head *sarp_head,
		struct list_head *smac_head, struct list_head *sadj_head) {

	struct list_head *arp_pos, *arp_next;
	struct list_head *mac_pos, *mac_next;
	struct list_head *adj_pos, *adj_next;
	struct arp_table *parp_t;
	struct mac_table *pmac_t;
	struct adj_table *padj_t;
	struct adj_table sadj;

	struct adj_table *adj_node;


//	ASSERT(sarp_head);
//	ASSERT(smac_head);
//	ASSERT(sadj_head);

	memset(&sadj, 0, sizeof(struct adj_table));

	/*给adj节点设置老化标志*/
	list_for_each_safe(adj_pos,adj_next,sadj_head)
	{
		padj_t = list_entry(adj_pos, struct adj_table, list);
		padj_t->counter = 1;
	}

	/*查找生成的新节点*/
	list_for_each_safe(arp_pos,arp_next,sarp_head)
	{
		parp_t = list_entry(arp_pos, struct arp_table, list);
		list_for_each_safe(mac_pos,mac_next,smac_head)
		{
			pmac_t = list_entry(mac_pos, struct mac_table, list);

			if (strcmp(parp_t->str_mac, pmac_t->str_mac) == 0
					&& strcmp(parp_t->str_vid, pmac_t->str_vid) == 0) {

				adj_node = adj_lookup(parp_t->str_vrf, parp_t->str_ip, parp_t->str_mac,
						parp_t->str_vid, pmac_t->str_interface,sadj_head);

				if(adj_node != NULL) {
					adj_node->counter = 0;
				}
//				strcpy(sadj.str_interface, pmac_t->str_interface);
//				strcpy(sadj.str_ip, parp_t->str_ip);
//				strcpy(sadj.str_vrf, parp_t->str_vrf);
//				strcpy(sadj.str_mac, parp_t->str_mac);
//				strcpy(sadj.str_vid, parp_t->str_vid);
//				add_adj_table(fp, &sadj,sadj_head);
			}
		}
	}

	/*删除老化节点*/
	del_adj_table(fp, sadj_head);

	memset(&sadj, 0, sizeof(struct adj_table));

	list_for_each_safe(arp_pos,arp_next,sarp_head)
	{
		parp_t = list_entry(arp_pos, struct arp_table, list);
		list_for_each_safe(mac_pos,mac_next,smac_head)
		{
			pmac_t = list_entry(mac_pos, struct mac_table, list);

			if (strcmp(parp_t->str_mac, pmac_t->str_mac) == 0
					&& strcmp(parp_t->str_vid, pmac_t->str_vid) == 0) {
				strcpy(sadj.str_interface, pmac_t->str_interface);
				strcpy(sadj.str_ip, parp_t->str_ip);
				strcpy(sadj.str_vrf, parp_t->str_vrf);
				strcpy(sadj.str_mac, parp_t->str_mac);
				strcpy(sadj.str_vid, parp_t->str_vid);
				add_adj_table(fp, &sadj,sadj_head);
			}
		}
	}

	return APP_SUCC;
}

