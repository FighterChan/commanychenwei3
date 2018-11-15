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

struct list_head arp_head;
struct list_head mac_head;
struct list_head adj_head;

#if !DEBUG

int flg;

int main(int argc, char **argv) {


	int i = 0;

	INIT_LIST_HEAD(&arp_head);

	INIT_LIST_HEAD(&mac_head);

	INIT_LIST_HEAD(&adj_head);

	char cmd[32];
	memset(cmd,0,sizeof(cmd));

	CLEAR_FLAG_ALL(flg);

	int adj_count = 0;

	FILE *infp;
	infp = fopen(argv[1],"r");
	ASSERT(infp);

	while(!feof(infp)) {
		fscanf(infp,"%s",cmd);
		if(strcmp(cmd,"add-arp") == 0) {
			add_arp_table(infp,&arp_head);
			SET_FLAG(flg,ADD_ARP);
		} else if (strcmp(cmd,"add-mac") == 0) {
			add_mac_table(infp,&mac_head);
			SET_FLAG(flg,ADD_MAC);
		} else if (strcmp(cmd,"del-arp") == 0){
			del_arp_table(infp,&arp_head);
			SET_FLAG(flg,DEL_ARP);
		} else if (strcmp(cmd,"del-mac") == 0) {
			del_mac_table(infp,&mac_head);
			SET_FLAG(flg,DEL_MAC);
		} else if (strcmp(cmd,"del-vrf") == 0) {
			del_table_by_vrf(infp,&arp_head,&adj_head);
			SET_FLAG(flg,DEL_VRF);
		} else if (strcmp(cmd,"del-vid") == 0) {
			del_table_by_vid(infp,&mac_head,&adj_head);
			SET_FLAG(flg,DEL_VID);
		} else if (strcmp(cmd,"show-adj-all") == 0) {
			SET_FLAG(flg,SHOW_ADJ_ALL);
		} else if (strcmp(cmd,"show-adj") == 0) {
			SET_FLAG(flg,SHOW_ADJ);
		} else if (strcmp(cmd,"show-log") == 0) {
			SET_FLAG(flg,SHOW_LOG);
		} else {
			memset(cmd,0,sizeof(cmd));
			CLEAR_FLAG_ALL(flg);
			continue;
		}

		/*arp和mac同时存在的时候才更新adj,删除任意表，都将更新adj*/
		if (CHECK_FLAG(flg,SHOW_LOG) | CHECK_FLAG(flg,SHOW_ADJ_ALL) | CHECK_FLAG(flg,SHOW_ADJ)) {
			look_up_node(&adj_count,&arp_head,&mac_head,&adj_head);
			write_file(argv[1],adj_count,&adj_head);
			break;
		}
	}	/* 输出adj到文件 */

	fclose(infp);
	free_arp_table(&arp_head);
	free_mac_table(&mac_head);
	free_adj_table(&adj_head);
	return APP_SUCC;
}
#endif

