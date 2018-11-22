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
init_mac_hash (void)
{
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            INIT_HLIST_HEAD(&mac_head[i]);
        }
    return APP_SUCC;
}

struct mac_table *
look_up_mac (struct mac_table *s)
{

    struct mac_table *p;
    struct hlist_node *n;
    u32 key;
    key = get_mac_key (s->int_vid, s->str_mac);
    if (hlist_empty (&mac_head[key]))
        {
//            printf ("没有该节点！\n");
            return NULL;
        }
    hlist_for_each_entry_safe(p, n, &mac_head[key],list)
        {
            /*加上某个条件后*/
            return p;
        }
    return NULL;
}

int
add_mac_table (struct mac_table *s)
{
    struct mac_table *p;
    struct hlist_node *n;
    u32 key;
    key = get_mac_key (s->int_vid, s->str_mac);
    p = look_up_mac (s);
    if (p == NULL)
        {
            p = (struct mac_table *) malloc (sizeof(struct mac_table));
            if (p == NULL)
                {
                    return APP_ERR;
                }
            copy_to_mac (p, s);
            hlist_add_head (&p->list, &mac_head[key]);
            printf("mac添加成功!\n");
            return APP_SUCC;
        }
//    printf ("mac重复值,不再添加!!!\n");
    return APP_ERR;
}

int
del_mac_table (struct mac_table *s)
{

    struct mac_table *p;
    struct hlist_node *n;
    u32 key;
    key = get_mac_key (s->int_vid, s->str_mac);
    if (hlist_empty (&mac_head[key]))
        {
            printf ("没有该节点！\n");
            return APP_ERR;
        }
    hlist_for_each_entry_safe(p, n, &mac_head[key],list)
        {
            /*加上某个条件后*/
            hlist_del (&p->list);
            return APP_SUCC;
        }
    return APP_ERR;
}

int
free_mac_table (void)
{
    struct mac_table *p;
    struct hlist_node *n;

    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            hlist_for_each_entry_safe(p,n,&mac_head[i],list)
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
    return (jhash_2words (vid, jhash (mac, strlen (mac), 0), 0) % HLIST_LEN_MAX);
}

int
copy_to_mac (struct mac_table *p, struct mac_table *s)
{

    p->int_vid = s->int_vid;
    strcpy (p->str_mac, s->str_mac);
    strcpy (p->str_interface, s->str_interface);
    return APP_SUCC;
}
