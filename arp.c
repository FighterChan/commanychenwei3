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
#include "adj.h"
#include "jhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int
init_arp_hash (void)
{
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            INIT_LIST_HEAD (&arp_head[i]);
        }
    return APP_SUCC;
}

struct arp_table *
look_up_arp (const char *vrf, const char *ip)
{

    struct arp_table *p;
    struct arp_table *n;
    u32 key;
    key = get_arp_key (vrf, ip);
    if (list_empty (&arp_head[key]))
        {
//            printf ("没有该节点！\n");
            return NULL;
        }
    list_for_each_entry_safe(p, n, &arp_head[key],list)
        {
            /*加上某个条件后*/
            return p;
        }
    return NULL;
}

int
add_arp_table (struct arp_table *s)
{
    struct arp_table *p = NULL;
    struct arp_table *n;
    u32 key;
    key = get_key (s->int_vid, s->str_mac);
//    p = look_up_arp (s);
    if (p == NULL)
        {
            p = (struct arp_table *) malloc (sizeof(struct arp_table));
            if (p == NULL)
                {
                    return APP_ERR;
                }
            copy_to_arp (p, s);
            list_add_tail (&p->list, &arp_head[key]);
//            printf ("arp添加成功!\n");
//            printf ("%s %s %s %d\n", p->str_vrf, p->str_ip, p->str_mac,
//                    p->int_vid);
            return APP_SUCC;
        }
    else
        {
            /*更新*/
            copy_to_arp (p, s);
            printf ("arp重复值,不再添加!!!\n");
//            printf ("%s %s %s %d\n", p->str_vrf, p->str_ip, p->str_mac,
//                    p->int_vid);
            return APP_SUCC;
        }
    return APP_ERR;
}

int
del_arp (FILE *fp,const char *vrf, const char *ip)
{
    struct adj_table *p;

    p = look_up_adj (vrf, ip);
    if (p == NULL)
        {
            return APP_ERR;
        }
    del_adj_table (fp,p->str_vrf, p->str_ip);
    return APP_SUCC;
}

int
free_arp_table (void)
{
    struct arp_table *p;
    struct arp_table *n;

    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            list_for_each_entry_safe(p,n,&arp_head[i],list)
                {
                    list_del_init (&p->list);
                    free (p);
                    p = NULL;
                }
        }
    return APP_SUCC;
}

u32
get_arp_key (const char *vrf, const char *ip)
{
    return (jhash_2words (jhash (vrf, strlen (vrf), HASH_INITVAL),
                          jhash (ip, strlen (ip), HASH_INITVAL), HASH_INITVAL)
            % HLIST_LEN_MAX);
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

