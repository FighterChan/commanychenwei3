/* 
 * main.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-10
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

int look_up_node(int show, const char *sel_vrf, int *adj_count,
		struct arp_table *arp_pos, struct list_head *sarp_head,
		struct mac_table *mac_pos, struct list_head *smac_head,
		struct adj_table *adj_pos, struct list_head *sadj_head);

static const char *cmd[CMD_MAX] = { "add-arp", "add-arp", "del-arp", "del-mac",
		"del-vrf", "del-vid", "show-adj-all", "show-adj", "show-log" };

int main(int argc, char **argv) {

	struct mac_table *pmac;
	struct arp_table *parp;
	struct adj_table *padj;
//
	struct list_head smac_head;
	struct list_head sarp_head;
	struct list_head sadj_head;
//
	struct list_head *arp_pos, *arp_next;
	struct list_head *mac_pos, *mac_next;
	struct list_head *adj_pos, *adj_next;
//
	memset(&sadj_head, 0, sizeof(struct list_head));
	memset(&smac_head, 0, sizeof(struct list_head));
	memset(&sarp_head, 0, sizeof(struct list_head));

	/* 初始化双链表的表头  */
	INIT_LIST_HEAD(&sarp_head);
	INIT_LIST_HEAD(&smac_head);
	INIT_LIST_HEAD(&sadj_head);

	char command[32 + 1];
	memset(command, 0, sizeof(command));

	char outfile[32 + 1];
	memset(outfile, 0, sizeof(outfile));

	char sel_vrf[32 + 1];
	memset(sel_vrf, 0, sizeof(sel_vrf));

	FILE *infp;

	int adj_count = 0;
	int show_mode = 0;
	int show_cmd = 0;

	if (argc < 2) {
		printf("argc error!!!\n");
		return APP_ERR;
	}
	infp = fopen(argv[1], "r");
	if (infp == NULL) {
		printf("cannot open input file!!!\n");
		return APP_ERR;
	}
	conver_filename(argv[1],outfile);

	while (!feof(infp)) {

		fscanf(infp, "%s", command);

		if (strcmp(command, "add-arp") == 0) {
			parp = (struct arp_table *) malloc(sizeof(struct arp_table));
			DEFINE_PARAM(parp);
			add_arp_table(infp, parp, &sarp_head);
		} else if (strcmp(command, "add-mac") == 0) {
			pmac = (struct mac_table *) malloc(sizeof(struct mac_table));
			DEFINE_PARAM(pmac);
			add_mac_table(infp, pmac, &smac_head);
		} else if (strcmp(command, "del-arp") == 0) {

		} else if (strcmp(command, "del-mac") == 0) {

		} else if (strcmp(command, "del-vrf") == 0) {

		} else if (strcmp(command, "del-vid") == 0) {

		} else if (strcmp(command, "show-adj-all") == 0) {
			show_mode = SHOW_ALL;
			look_up_node(outfile,SHOW_ALL,NULL,arp_pos,&sarp_head,mac_pos,&smac_head,adj_pos,&sadj_head);
		} else if (strcmp(command, "show-adj") == 0) {
			show_mode = SHOW_SELECT;
			show_cmd = 1;
			char *vrf_name = (char *) malloc(48);
			if (!vrf_name) {
				printf("vrf_name malloc error!\n");
				return -1;
			}
			fscanf(infp, "%s", vrf_name);
			char *token;
			int index = 0;
			while (token = strsep(&vrf_name, "=")) {
				if (index == 1) {
					strcpy(sel_vrf, token);
				}
				index++;
			}
			free(vrf_name);
		} else if (strcmp(command, "show-log") == 0) {

		}

		/*清除上一条读入的string*/
		memset(command, 0, sizeof(command));
	}
	fclose(infp);


	{
		/*释放资源*/
		list_for_each_safe(arp_pos,arp_next,&sarp_head)
		{
			parp = list_entry(arp_pos, struct arp_table, list);
			list_del_init(arp_pos);
			free(parp);
		}

		list_for_each_safe(mac_pos,mac_next,&smac_head)
		{
			pmac = list_entry(mac_pos, struct mac_table, list);
			list_del_init(mac_pos);
			free(pmac);
		}

		list_for_each_safe(adj_pos,adj_next,&sadj_head)
		{
			padj = list_entry(adj_pos, struct adj_table, list);
			list_del_init(adj_pos);
			free(padj);
		}
	}

	return APP_SUCC;
}

int read_file(const char *infile) {
	FILE *infp;

	_ASSERT(infile);

	infp = fopen(infile, "r");

	_ASSERT(infp);

	while (!feof(infp)) {

	}
	return APP_SUCC;
}

int write_file(FILE *fp,int adj_count,struct adj_table *adj_pos,
		struct list_head *sadj_head) {

	fprintf(fp,"count:%d\n", adj_count);

	/*输出*/
	printf("count:%d\n", adj_count);
	list_for_each(adj_pos,sadj_head) {
		padj = list_entry(adj_pos, struct adj_table, list);
		fprintf(fp,"%s %s %s %s %s\n", padj->str_vrf, padj->str_ip,
				padj->str_mac, padj->str_vid, padj->str_interface);
	}
}
/**
 * 遍历函数
 */

int look_up_node(const char *outfile,int show, const char *sel_vrf,
		struct arp_table *arp_pos, struct list_head *sarp_head,
		struct mac_table *mac_pos, struct list_head *smac_head,
		struct adj_table *adj_pos, struct list_head *sadj_head) {

	int count = 0;
	list_for_each(arp_pos,sarp_head) {
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

int free_list() {
	return APP_SUCC;
}

