/* 
 * app.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-13
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include "adj.h"
#include "arp.h"
#include "list.h"
#include "mac.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct table_node *pnode;

//
struct list_head head[3];
//



int main(int argc, char **argv) {

	memset(head, 0, sizeof(head));

	/* 初始化双链表的表头  */
	int i;
	for(i = 0;i < 3;i ++) {
		INIT_LIST_HEAD(&head[i]);
	}

	pnode->padj = (struct adj_table *)malloc(sizeof(struct adj_table));
	if(!pnode->padj) {
		return APP_ERR;
	}

	i = 0;
	for(i = 0;i < 10;i ++) {
		sprintf(pnode->padj->str_interface,"count:%d",i);
		list_add_tail(&pnode->padj->list,&head[0]);
	}

	list_for_each(pnode->adj_pos,&head[0]){
		pnode->padj = list_entry(pnode->padj,struct adj_table,list);
		printf("%s\n",pnode->padj->str_interface);
	}


	return APP_SUCC;
}

int look_up_node(const char *outfile,struct table_node *pnode,struct list_head *phead,int show, const char *sel_vrf) {

	int count = 0;
	list_for_each(pnode->arp_pos,sarp_head) {
		parp = list_entry(arp_pos, struct arp_table, list);
		list_for_each(mac_pos,smac_head) {
			pmac = list_entry(mac_pos, struct mac_table, list);
			if ((strcmp(parp->str_mac, pmac->str_mac) == 0
					&& strcmp(parp->str_vid, pmac->str_vid) == 0)
					&& (show || strcmp(sel_vrf, parp->str_vrf) == 0)) {
				padj = (struct adj_table *) malloc(sizeof(struct adj_table));
				if (!padj) {
					printf("padj malloc error!\n");
					return APP_ERR;
				}
				strcpy(padj->str_vrf, parp->str_vrf);
				strcpy(padj->str_ip, parp->str_ip);
				strcpy(padj->str_mac, parp->str_mac);
				strcpy(padj->str_vid, parp->str_vid);
				strcpy(padj->str_interface, pmac->str_interface);
				list_add_tail(&padj->list, sadj_head);
				count++;
			}
		}
	}

	FILE *outfp;
	outfp = fopen(outfile, "w");
	_ASSERT(outfp);
	/* 输出adj到文件 */
	write_file(outfp,count,adj_pos,sadj_head);
	free(outfp);
	return APP_SUCC;
}
