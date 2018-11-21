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
#include "list.h"
#include "app.h"
#include "arp.h"
#include "jhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
add_arp_table (struct arp_table *s, struct hlist_head *head)
{
    u32 key;
    key = get_arp_key (s->str_vrf, s->str_ip);

    hlist_add_head (&s->list, &head[key]);
    return APP_SUCC;
}

int
del_arp_table (struct arp_table *s, struct hlist_head *head)
{
    struct arp_table *p;
    struct hlist_node *n;
    u32 key;
    key = get_arp_key (s->str_vrf, s->str_ip);
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
                    free (p);
                    p = NULL;
                }
        }

    return APP_SUCC;
}

int
free_arp_table (struct hlist_head *head)
{
    struct arp_table *p;
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
get_arp_key (const char *vrf, const char *ip)
{
    return (jhash_2words (jhash (vrf, strlen (vrf), 0),
                          jhash (ip, strlen (ip), 0)) % HLIST_LEN_MAX);
}

