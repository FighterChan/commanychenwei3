/* 
 * t.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-22
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
#include "jhash.h"
//#include <arpa/inet.h>

u32
get_arp_key (const char *vrf, const char *ip)
{
    char str[64];
    memset(str,0,sizeof(str));
    sprintf(str,"%s+%s",vrf,ip);
    return (jhash (str, strlen (str), 0) % 1024);
}


char vrf[] = "abc";
char ip[] = "10.0.3.1";

char vrf1[] = "abc";
char ip1[] = "10.0.3.10";

int
main (int argc, char **argv)
{
    printf("hash : %ld\n",get_arp_key(vrf,ip));
    printf("hash1 : %ld\n",get_arp_key(vrf1,ip1));

    return 0;
}
