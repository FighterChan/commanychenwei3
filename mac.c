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
#include "app.h"
#include "mac.h"
#include "jhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
add_arp_table (struct mac_table *s, struct hlist_head *head)
{
    u32 key;
    key = get_mac_key (s->ini_vid, s->str_mac);

    hlist_add_head (&s->list, &head[key]);
    return APP_SUCC;
}

int
del_mac_table (struct mac_table *s, struct hlist_head *head)
{
    struct mac_table *p;
    struct hlist_node *n;
    u32 key;
    key = get_mac_key (s->ini_vid, s->str_mac);
    if (hlist_empty (&head[key]))
        {
            printf ("data not exist \n");
            return APP_ERR;
        }
    else
        {
            hlist_for_each_entry_safe(p,n,&head[key],list)
                {
                    hlist_del (&p->list);
                    free(p);
                    p = NULL;
                }
        }

    return APP_SUCC;
}

int
free_mac_table (struct hlist_head *head)
{
    struct mac_table *p;
    struct hlist_node *n;

    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            hlist_for_each_entry_safe(p,n,&head[i],list)
                {
                    hlist_del (&p->list);
                    free (p);
                    p = NULL;
                }
        }
    return APP_SUCC;
}

u32
get_mac_key (u32 vid, const char *mac)
{
    return (jhash_2words (vid, jhash (mac, strlen (mac), 0)) % HLIST_LEN_MAX);
}
