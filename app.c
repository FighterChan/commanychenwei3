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

int look_up_node(const char *outfile, struct arp_table *parp_t,
		struct list_head *sarp_head, struct mac_table *pmac_t,
		struct list_head *smac_head, struct adj_table *padj_t,
		struct list_head *sadj_head);

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

struct arp_table *parp;
struct mac_table *pmac;
struct adj_table *padj;

struct list_head arp_head;
struct list_head mac_head;
struct list_head adj_head;

int main(int argc, char **argv) {



	INIT_LIST_HEAD(&arp_head);

	INIT_LIST_HEAD(&mac_head);

	MALLOC(struct adj_table,padj);
	INIT_LIST_HEAD(&adj_head);

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
			MALLOC(struct arp_table,parp);
			add_arp_table(infp,parp,&arp_head);
		} else if (strcmp(cmd,"add-mac") == 0) {
			MALLOC(struct mac_table,pmac);
			add_mac_table(infp,pmac,&mac_head);
		} else if (strcmp(cmd,"show-adj-all") == 0) {
			look_up_node(outpath,parp,&arp_head,pmac,&mac_head,padj,&adj_head);
		}
		memset(cmd,0,sizeof(cmd));
	}

	free_arp_table(parp,&arp_head);
	free_mac_table(pmac,&mac_head);
	free_adj_table(padj,&adj_head);
	return APP_SUCC;
}

int look_up_node(const char *outfile, struct arp_table *parp_t,
		struct list_head *sarp_head, struct mac_table *pmac_t,
		struct list_head *smac_head, struct adj_table *padj_t,
		struct list_head *sadj_head) {

	int count = 0;
	struct list_head *arp_pos;
	struct list_head *mac_pos;
	struct list_head *adj_pos;

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

	FILE *outfp;
	outfp = fopen(outfile, "w");
	ASSERT(outfp);
	/* 输出adj到文件 */
	write_file(outfp,count,padj_t,sadj_head);
	/*输出*/
	fclose(outfp);
	return APP_SUCC;
}
