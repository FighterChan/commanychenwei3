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
#include "tools.h"
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

	char outpath[32];
	memset(outpath,0,sizeof(outpath));

	int show_log = CLOSE_LOG;

	CLEAR_FLAG_ALL(flg);

	int adj_count = 0;

	FILE *infp;
	infp = fopen(argv[1],"r");
	ASSERT(infp);

	conver_filename(argv[1],outpath);

	FILE *outfp;
	outfp = fopen(outpath,"w");
	ASSERT(outfp);

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
			del_table_by_vrf(infp,outfp,show_log,&arp_head,&adj_head);
			SET_FLAG(flg,DEL_VRF);
		} else if (strcmp(cmd,"del-vid") == 0) {
			del_table_by_vid(infp,outfp,show_log,&mac_head,&adj_head);
			SET_FLAG(flg,DEL_VID);
		} else if (strcmp(cmd,"show-adj-all") == 0) {
			SET_FLAG(flg,SHOW_ADJ_ALL);
		} else if (strcmp(cmd,"show-adj") == 0) {
			SET_FLAG(flg,SHOW_ADJ);
		} else if (strcmp(cmd,"show-log") == 0) {
			show_log = OPEN_LOG;
		} else {

		}
		look_up_node(&adj_count, &arp_head, &mac_head, &adj_head);

		if (CHECK_FLAG(flg, SHOW_LOG)
				!= 0 || CHECK_FLAG(flg,SHOW_ADJ_ALL) != 0 || CHECK_FLAG(flg,SHOW_ADJ!= 0)) {
			write_file(outfp, show_log, adj_count, &adj_head);
			printf("i = %d\n", i++);
		}
		memset(cmd,0,sizeof(cmd));
	}	/* 输出adj到文件 */

	fclose(infp);
	fclose(outfp);
	free_arp_table(&arp_head);
	free_mac_table(&mac_head);
	free_adj_table(&adj_head);
	return APP_SUCC;
}
#endif

