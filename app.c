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

	memset(cmd,0,sizeof(cmd));

	FILE *infp;
	infp = fopen(argv[1],"r");
	ASSERT(infp);
	while(!feof(infp)) {
		fscanf(infp,"%s",cmd);
		if(strcmp(cmd,"add-arp") == 0) {
			add_arp_table(infp,pnode);
		}
	}
	MALLOC_FREE(pnode);
	return APP_SUCC;
}

//int look_up_node(const char *outfile,struct table_node *pnode,struct list_head *phead,int show, const char *sel_vrf) {
//
//	int count = 0;
//	list_for_each(pnode->arp_pos,sarp_head) {
//		parp = list_entry(arp_pos, struct arp_table, list);
//		list_for_each(mac_pos,smac_head) {
//			pmac = list_entry(mac_pos, struct mac_table, list);
//			if ((strcmp(parp->str_mac, pmac->str_mac) == 0
//					&& strcmp(parp->str_vid, pmac->str_vid) == 0)
//					&& (show || strcmp(sel_vrf, parp->str_vrf) == 0)) {
//				padj = (struct adj_table *) malloc(sizeof(struct adj_table));
//				if (!padj) {
//					printf("padj malloc error!\n");
//					return APP_ERR;
//				}
//				strcpy(padj->str_vrf, parp->str_vrf);
//				strcpy(padj->str_ip, parp->str_ip);
//				strcpy(padj->str_mac, parp->str_mac);
//				strcpy(padj->str_vid, parp->str_vid);
//				strcpy(padj->str_interface, pmac->str_interface);
//				list_add_tail(&padj->list, sadj_head);
//				count++;
//			}
//		}
//	}
//
//	FILE *outfp;
//	outfp = fopen(outfile, "w");
//	_ASSERT(outfp);
//	/* 输出adj到文件 */
//	write_file(outfp,count,adj_pos,sadj_head);
//	free(outfp);
//	return APP_SUCC;
//}
