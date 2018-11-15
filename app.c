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

int main(int argc, char **argv) {



	INIT_LIST_HEAD(&arp_head);

	INIT_LIST_HEAD(&mac_head);

	INIT_LIST_HEAD(&adj_head);

	char cmd[32];
	memset(cmd,0,sizeof(cmd));

	FILE *infp;
	infp = fopen(argv[1],"r");
	ASSERT(infp);

	while(!feof(infp)) {
		fscanf(infp,"%s",cmd);
		if(strcmp(cmd,"add-arp") == 0) {
			add_arp_table(infp,&arp_head);
		} else if (strcmp(cmd,"add-mac") == 0) {
			add_mac_table(infp,&mac_head);
		} else if (strcmp(cmd,"del-arp") == 0){
			del_arp_table(infp,&arp_head);
		} else if (strcmp(cmd,"del-mac") == 0) {
			del_mac_table(infp,&mac_head);
		} else if (strcmp(cmd,"show-adj-all") == 0) {
			look_up_node(argv[1],&arp_head,&mac_head,&adj_head);
		}
		memset(cmd,0,sizeof(cmd));
	}
	fclose(infp);
	free_arp_table(&arp_head);
	free_mac_table(&mac_head);
	free_adj_table(&adj_head);
	return APP_SUCC;
}
#endif

