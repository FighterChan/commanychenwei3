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
look_up_arp (struct arp_table *s)
{

    struct arp_table *p;
    struct arp_table *n;
    u32 key;
    key = get_arp_key (s->str_vrf, s->str_ip);
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
    key = get_arp_key (s->str_vrf, s->str_ip);
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
            printf ("%s %s %s %d\n", p->str_vrf, p->str_ip, p->str_mac,
                    p->int_vid);
            return APP_SUCC;
        }
    else
        {
            /*更新*/
            copy_to_arp (p, s);
            printf ("arp重复值,不再添加!!!\n");
            printf ("%s %s %s %d\n", p->str_vrf, p->str_ip, p->str_mac,
                    p->int_vid);
            return APP_SUCC;
        }
    return APP_ERR;
}

int
del_arp_table (struct arp_table *s)
{

    struct arp_table *p;
    struct arp_table *n;
    u32 key;
    key = get_arp_key (s->str_vrf, s->str_ip);
    if (list_empty (&arp_head[key]))
        {
//            printf ("没有该节点！\n");
            return APP_ERR;
        }
    list_for_each_entry_safe(p, n, &arp_head[key],list)
        {
            /*加上某个条件后*/
            list_del_init (&p->list);
            return APP_SUCC;
        }
    return APP_ERR;
}

int
del_arp_table_by_vrf (const char *vrf)
{

    struct arp_table *p;
    struct arp_table *n;
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {

            if (!list_empty (&arp_head[i]))
                {
                    list_for_each_entry_safe(p, n, &arp_head[i],list)
                        {
                            /*加上某个条件后*/
                            if (strcmp (p->str_vrf, vrf) == 0)
                                {
                                    printf("%s\n",p->str_vrf);
                                    list_del_init (&p->list);
                                    free(p);
                                    p = NULL;
                                }
                            return APP_SUCC;
                        }
                }

        }
    return APP_ERR;
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
                          ntohl (inet_addr (ip)), HASH_INITVAL) % HLIST_LEN_MAX);
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

