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
init_arp_hash (void)
{
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            INIT_HLIST_HEAD(&arp_head[i]);
        }
    return APP_SUCC;
}

struct arp_table *
look_up_arp (struct arp_table *s)
{

    struct arp_table *p;
    struct hlist_node *n;
    u32 key;
    key = get_arp_key (s->str_vrf, s->str_ip);
    if (hlist_empty (&arp_head[key]))
        {
            printf ("没有该节点！\n");
            return NULL;
        }
    hlist_for_each_entry_safe(p, n, &arp_head[key],list)
        {
            /*加上某个条件后*/
            return p;
        }
    return NULL;
}

int
add_arp_table (struct arp_table *s)
{
    struct arp_table *p;
    struct hlist_node *n;
    u32 key;
    key = get_arp_key (s->str_vrf, s->str_ip);
    p = look_up_arp (s);
    if (p == NULL)
        {
            p = (struct arp_table *) malloc (sizeof(struct arp_table));
            if (p == NULL)
                {
                    return APP_ERR;
                }
            copy_to_arp (p, s);
            hlist_add_head (&p->list, &arp_head[key]);
            return APP_SUCC;
        }
    printf ("arp重复值,不再添加!!!\n");
    return APP_ERR;
}

int
del_arp_table (struct arp_table *s)
{

    struct arp_table *p;
    struct hlist_node *n;
    u32 key;
    key = get_arp_key (s->str_vrf, s->str_ip);
    if (hlist_empty (&arp_head[key]))
        {
            printf ("没有该节点！\n");
            return APP_ERR;
        }
    hlist_for_each_entry_safe(p, n, &arp_head[key],list)
        {
            /*加上某个条件后*/
            hlist_del (&p->list);
            return APP_SUCC;
        }
    return APP_ERR;
}

int
free_arp_table (void)
{
    struct arp_table *p;
    struct hlist_node *n;

    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            hlist_for_each_entry_safe(p,n,&arp_head[i],list)
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
                          jhash (ip, strlen (ip), 0), 0) % HLIST_LEN_MAX);
}

int
copy_to_arp (struct arp_table *p, struct arp_table *s)
{

    strcpy (p->str_vrf, s->str_vrf);
    strcpy (p->str_ip, s->str_ip);
    p->int_vid = s->int_vid;
    strcpy (p->str_mac, s->str_mac);
    return APP_SUCC;
}

