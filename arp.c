/* 
 * arp.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-10
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include "arp.h"
#include "list.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@ int add_arp_table (FILE *fp,int index,struct arp_table *arp);
 *@ function:
 */

int add_arp_table(FILE *infp, struct arp_table *parp, struct list_head *head) {

	fscanf(infp, "%s%s%s%s", parp->str_vrf, parp->str_ip, parp->str_mac,
			parp->str_vid);

	list_add_tail(&parp->list, head);

	return APP_SUCC;
}

