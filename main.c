/* 
 * main.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-16
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include <stdio.h>
#include <string.h>
#include "list.h"
#include "jhash.h"
#include "app.h"

#define LIST_SIZE (1024)

struct list_head arp_head[LIST_SIZE];
struct list_head mac_head[LIST_SIZE];
struct list_head adj_head[LIST_SIZE];

u32 get_arp_hash(const char *vrf, const char *ip) {

	return jhash_2words(jhash(vrf, strlen(vrf), 0), jhash(ip, strlen(ip), 0), 0)
			% LIST_SIZE;
}

u32 get_mac_hash(u32 vid, const char *mac) {

	return jhash_2words(vid, jhash(mac, strlen(mac), 0), 0) % LIST_SIZE;
}

u32 get_adj_hash(const char *vrf, const char *ip) {

	return jhash_2words(jhash(vrf, strlen(vrf), 0), jhash(ip, strlen(ip), 0), 0)
			% LIST_SIZE;
}

int arp_init(void) {

	int i;
	for (i = 0; i < LIST_SIZE; i++) {
		INIT_LIST_HEAD(&arp_head[i]);
	}
	return 0;
}

int mac_init(void) {

	int i;
	for (i = 0; i < LIST_SIZE; i++) {
		INIT_LIST_HEAD(&mac_head[i]);
	}
	return 0;
}

int adj_init(void) {

	int i;
	for (i = 0; i < LIST_SIZE; i++) {
		INIT_LIST_HEAD(&adj_head[i]);
	}
	return 0;
}

int add_arp_node(FILE *fp) {

	u32 hash;
	int vid;
	char vrf[32 + 1];
	char mac[16 + 1];
	char ip[16 + 1];

	struct arp_table *parp;


	fscanf(fp, "%s%s%s%d", vrf, ip, mac, &vid);

	hash = get_arp_hash(vrf, ip);

#if 0
	list_for_each_entry(parp,&arp_head[hash],list)
	{
		if (parp && vid == parp->int_vid && strcmp(parp->str_mac,mac) == 0 &&
				strcmp(parp->str_ip,ip) == 0 && strcmp(parp->str_vrf,vrf) == 0) {
			return -1;
		}
	}
#endif

	MALLOC(struct arp_table, parp);

	strcpy(parp->str_vrf,vrf);
	strcpy(parp->str_ip,ip);
	strcpy(parp->str_mac,mac);
	parp->int_vid = vid;

	list_add_tail(&parp->list, &arp_head[hash]);
	return 0;
}

int add_mac_node(FILE *fp) {

	u32 hash;
	int vid;
	char mac[16 + 1];
	char interface[32 + 1];

	struct mac_table *pmac;

	fscanf(fp, "%d%s%s", &vid, mac, interface);

	hash = get_mac_hash(vid, mac);

#if 0
	list_for_each_entry(pmac,&mac_head[hash],list)
	{
		if (pmac && vid == pmac->int_vid && strcmp(pmac->str_mac,mac) == 0 &&
				strcmp(pmac->str_interface,interface) == 0) {
			return -1;
		}
	}
#endif

	MALLOC(struct mac_table, pmac);

	pmac->int_vid = vid;
	strcpy(pmac->str_mac, mac);
	strcpy(pmac->str_interface, interface);

	list_add_tail(&pmac->list, &mac_head[hash]);

	return 0;
}

int ii = 0,iii = 0;
int add_adj_node(void) {

	u32 arp_hash;
	u32 mac_hash;
	u32 adj_hash;
	struct adj_table *padj;
	struct mac_table *pmac;
	struct arp_table *parp;

	int i;
	for (i = 0; i < LIST_SIZE; ++i) {
		list_for_each_entry(parp,&arp_head[i],list)
		{
			mac_hash = get_mac_hash(parp->int_vid, parp->str_mac);
			list_for_each_entry(pmac,&mac_head[mac_hash],list)
			{

				adj_hash = get_adj_hash(parp->str_vrf, parp->str_ip);
#if 1
				list_for_each_entry(padj,&adj_head[adj_hash],list)
				{
					if (padj && strcmp(padj->str_vrf, parp->str_vrf) == 0
							&& strcmp(padj->str_ip, parp->str_ip) == 0
							&& strcmp(padj->str_mac, parp->str_mac) == 0
							&& padj->int_vid == parp->int_vid
							&& strcmp(padj->str_interface, pmac->str_interface)
									== 0) {
						printf("iii = %d\n",iii++);

						return -1;

					}
				}
#endif
				MALLOC(struct adj_table, padj);
				strcpy(padj->str_vrf, parp->str_vrf);
				strcpy(padj->str_ip, parp->str_ip);
				strcpy(padj->str_mac, parp->str_mac);
				padj->int_vid = parp->int_vid;
				strcpy(padj->str_interface, pmac->str_interface);
				list_add_tail(&padj->list, &adj_head[adj_hash]);
				printf("index = %d\n",ii++);
			}
		}
	}
	return 0;
}

int show_adj(void) {

	return 0;
}

int main(int argc, char **argv) {

	char cmd[32];
	memset(cmd, 0, sizeof(cmd));

	arp_init();
	mac_init();
	adj_init();

	FILE *infp;
	infp = fopen(argv[1], "r");
	ASSERT(infp);

	while (!feof(infp)) {
		fscanf(infp, "%s", cmd);
		if (strcmp(cmd, "add-arp") == 0) {
			add_arp_node(infp);
		} else if (strcmp(cmd, "add-mac") == 0) {
			add_mac_node(infp);
		}
		add_adj_node();
		memset(cmd, 0, sizeof(cmd));
	}

	struct adj_table *padj;
	FILE *outfp;
	outfp = fopen("out.txt","w");
	ASSERT(outfp);
	int i;
	for (i = 0; i < LIST_SIZE; ++i) {
		list_for_each_entry(padj,&adj_head[i],list)
		{
			fprintf(outfp,"%s %s %s %d %s\n", padj->str_vrf, padj->str_ip,
					padj->str_mac, padj->int_vid, padj->str_interface);
		}
	}

	return APP_SUCC;
}

