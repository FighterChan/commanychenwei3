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
#include "adj.h"

int
init_mac_hash (void)
{
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            INIT_LIST_HEAD (&mac_head[i]);
        }
    return APP_SUCC;
}

struct mac_table *
look_up_mac (int vid, const char *mac)
{

    struct mac_table *p;
    struct mac_table *n;
    u32 key;
    key = get_mac_key (vid, mac);
    if (list_empty (&mac_head[key]))
        {
//            printf ("没有该节点！\n");
            return NULL;
        }
    list_for_each_entry_safe(p, n, &mac_head[key],list)
        {
            /*加上某个条件后*/
            return p;
        }
    return NULL;
}

int
add_mac_table (struct mac_table *s)
{
    struct mac_table *p = NULL;
    struct mac_table *n;
    u32 key;
    key = get_mac_key (s->int_vid, s->str_mac);
//    p = look_up_mac (s);
    if (p == NULL)
        {
            p = (struct mac_table *) malloc (sizeof(struct mac_table));
            if (p == NULL)
                {
                    return APP_ERR;
                }
            copy_to_mac (p, s);
            list_add_tail (&p->list, &mac_head[key]);
//            printf ("mac添加成功!\n");
//            printf ("%s %d %s\n", p->str_mac, p->int_vid, p->str_interface);
            return APP_SUCC;
        }
    else
        {
            /*更新*/
            copy_to_mac (p, s);
            printf ("mac重复值,不再添加!!!\n");
            return APP_SUCC;
        }
    return APP_ERR;
}

int
del_mac_table (int vid, const char *mac)
{
    struct arp_table *parp,*narp;
    struct mac_table *pmac;
    struct adj_table sadj;
    int arp_key = 0;

    pmac = look_up_mac (vid, mac);
    if (pmac == NULL)
        {
            printf ("mac表中找不到该节点!\n");
            return APP_ERR;
        }
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            if (!list_empty (&arp_head[i]))
                {
                    list_for_each_entry_safe(parp,narp,&arp_head[i],list)
                        {
                            if (parp->int_vid == pmac->int_vid
                                    && strcmp (parp->str_mac, pmac->str_mac)
                                            == 0)
                                {
                                    del_adj_table(parp->str_vrf,parp->str_ip);
                                }

                        }
                }
        }

    return APP_SUCC;
}

int
free_mac_table (void)
{
    struct mac_table *p;
    struct mac_table *n;

    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            list_for_each_entry_safe(p,n,&mac_head[i],list)
                {
                    list_del_init (&p->list);
                    free (p);
                    p = NULL;
                }
        }
    return APP_SUCC;
}

u32
get_mac_key (u32 vid, const char *mac)
{
    return (jhash_2words (vid, jhash (mac, strlen (mac), HASH_INITVAL),
    HASH_INITVAL) % HLIST_LEN_MAX);
}

int
copy_to_mac (struct mac_table *p, struct mac_table *s)
{

    p->int_vid = s->int_vid;
    strcpy (p->str_mac, s->str_mac);
    strcpy (p->str_interface, s->str_interface);
    return APP_SUCC;
}
