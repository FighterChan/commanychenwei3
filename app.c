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
#include "app.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct table_node *pnode;
char cmd[32];

int main(int argc, char **argv) {


	pnode = (struct table_node *)malloc(sizeof(struct table_node));
	ASSERT(pnode);

	pnode->parp = (struct arp_node *)malloc(sizeof(struct arp_node));
	ASSERT(pnode->parp);
	INIT_LIST_HEAD(&pnode->parp->head);

	pnode->pmac = (struct mac_node *)malloc(sizeof(struct mac_node));
	ASSERT(pnode->pmac);
	INIT_LIST_HEAD(&pnode->pmac->head);

	pnode->padj = (struct adj_node *)malloc(sizeof(struct adj_node));
	ASSERT(pnode->padj);
	INIT_LIST_HEAD(&pnode->padj->head);



	memset(cmd,0,sizeof(cmd));

	FILE *infp;
	infp = fopen(argv[1],"r");
	ASSERT(infp);
	while(!feof(infp)) {
		fscanf(infp,"%s",cmd);
		if(strcmp(cmd,"add-arp") == 0) {
			add_arp_table(infp,pnode->parp);
		} else if (strcmp(cmd,"add-mac") == 0) {
			add_mac_table(infp,pnode->pmac);
		} else if (strcmp(cmd,"show-adj-all") == 0) {

		}
//		look_up_node(NULL,pnode);
		memset(cmd,0,sizeof(cmd));
	}
//	MALLOC_FREE(pnode->parp);
	MALLOC_FREE(pnode);
	return APP_SUCC;
}

int look_up_node(const char *outfile,struct table_node *pnode) {

	int count = 0;
	struct arp_node *parp = pnode->parp;
	struct mac_node *pmac = pnode->pmac;
	struct adj_node *padj = pnode->padj;

	list_for_each(parp->pos,&parp->head) {
		parp->node = list_entry(parp->pos, struct arp_table, list);
		list_for_each(pmac->pos,&pmac->head) {
			pmac->node = list_entry(pmac->pos, struct mac_table, list);
			if ((strcmp(parp->node->str_mac, pmac->node->str_mac) == 0
					&& strcmp(parp->node->str_vid, pmac->node->str_vid) == 0)) {
				padj->node = (struct adj_table *) malloc(sizeof(struct adj_table));
				ASSERT(padj->node);

				strcpy(padj->node->str_vrf, parp->node->str_vrf);
				strcpy(padj->node->str_ip, parp->node->str_ip);
				strcpy(padj->node->str_mac, parp->node->str_mac);
				strcpy(padj->node->str_vid, parp->node->str_vid);
				strcpy(padj->node->str_interface, pmac->node->str_interface);
				list_add_tail(&padj->node->list, &padj->head);
				count++;
			}
		}
	}



//	FILE *outfp;
//	outfp = fopen(outfile, "w");
//	ASSERT(outfp);
//	/* 输出adj到文件 */
//	write_file(outfp,count,adj_pos,sadj_head);
//	free(outfp);
	return APP_SUCC;
}
