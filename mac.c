/* 
 * mac.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-10
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include "list.h"
#include "mac.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @int add_mac_table (FILE *fp,int index,struct mac_table *arp);
 * @fun
 *
 *
 */
int add_mac_table(FILE *infp,struct mac_table *pmac,struct list_head *smac_head) {

	fscanf(infp, "%s%s%s", pmac->str_vid, pmac->str_mac,
			pmac->str_interface);
	list_add_tail(&pmac->list, smac_head);
	return APP_SUCC;
}
