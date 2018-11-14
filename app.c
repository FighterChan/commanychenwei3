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

int look_up_node(const char *outfile);

int conver_filename(char *infile, char *outfile) {
	char *token;
	char *outpath = outfile;
	if ((token = strsep(&infile, ".")) != NULL) {
		sprintf(outpath, "%s%s", token, "_result.txt");
	} else {
		return APP_ERR;
	}
	return APP_SUCC;
}

struct arp_node *parp;
struct mac_node *pmac;
struct adj_node *padj;

int main(int argc, char **argv) {



	MALLOC(struct arp_node,parp);
	INIT_LIST_HEAD(&parp->head);

	MALLOC(struct mac_node,pmac);
	INIT_LIST_HEAD(&pmac->head);

	MALLOC(struct adj_node,padj);
	INIT_LIST_HEAD(&padj->head);

	char cmd[32];
	memset(cmd,0,sizeof(cmd));

	char outpath[32];
	memset(outpath,0,sizeof(outpath));

	FILE *infp;
	infp = fopen(argv[1],"r");
	ASSERT(infp);

	conver_filename(argv[1],outpath);

	while(!feof(infp)) {
		fscanf(infp,"%s",cmd);
		if(strcmp(cmd,"add-arp") == 0) {
			MALLOC(struct arp_table,parp->node);
			add_arp_table(infp,parp->node,&parp->head);
		} else if (strcmp(cmd,"add-mac") == 0) {
			MALLOC(struct mac_table,pmac->node);
			add_mac_table(infp,pmac->node,&pmac->head);
		} else if (strcmp(cmd,"show-adj-all") == 0) {
			look_up_node(outpath);
		}
		memset(cmd,0,sizeof(cmd));
	}

	free_arp_table(parp->node,&parp->head);
	MALLOC_FREE(parp);
	free_mac_table(pmac->node,&pmac->head);
	MALLOC_FREE(pmac);
	free_adj_table(padj->node,&padj->head);
	MALLOC_FREE(padj);
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

int look_up_node(const char *outfile) {

	int count = 0;
	struct list_head *arp_pos;
	struct list_head *mac_pos;
	struct list_head *adj_pos;

	list_for_each(arp_pos,&parp->head) {
		parp->node = list_entry(arp_pos, struct arp_table, list);
		list_for_each(mac_pos,&pmac->head) {
			pmac->node = list_entry(mac_pos, struct mac_table, list);
			if ((strcmp(parp->node->str_mac, pmac->node->str_mac) == 0
					&& strcmp(parp->node->str_vid, pmac->node->str_vid) == 0)) {
				MALLOC(struct adj_node,padj->node);
				strcpy(padj->node->str_vrf, parp->node->str_vrf);
				strcpy(padj->node->str_ip, parp->node->str_ip);
				strcpy(padj->node->str_mac, parp->node->str_mac);
				strcpy(padj->node->str_vid, parp->node->str_vid);
				strcpy(padj->node->str_interface, pmac->node->str_interface);
				list_add_tail(&padj->node->list, &padj->head);
				printf("%s %s %s %s %s\n", padj->node->str_vrf, padj->node->str_ip,
								padj->node->str_mac, padj->node->str_vid, padj->node->str_interface);
				count++;
			}
		}
	}

	FILE *outfp;
	outfp = fopen(outfile, "w");
	ASSERT(outfp);
	/* 输出adj到文件 */
//	write_file(outfp,count,padj->node,&padj->head);
	fprintf(outfp,"count:%d\n", count);
	printf("count:%d\n", count);
	/*输出*/
	list_for_each(adj_pos,&padj->head) {
		padj->node = list_entry(adj_pos, struct adj_table, list);
		fprintf(outfp,"%s %s %s %s %s\n", padj->node->str_vrf, padj->node->str_ip,
				padj->node->str_mac, padj->node->str_vid, padj->node->str_interface);
	}
	free(outfp);
	return APP_SUCC;
}
