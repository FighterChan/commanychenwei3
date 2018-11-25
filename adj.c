/* 
 * adj.c
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
#include "adj.h"
#include "mac.h"
#include "app.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int
init_adj_hash (void)
{
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            INIT_LIST_HEAD (&adj_head[i]);
        }
    return APP_SUCC;
}

struct adj_table *
look_up_adj (const char *vrf, const char *ip)
{

    struct adj_table *p;
    struct adj_table *n;
    u32 key;
    key = get_adj_key (vrf, ip);
    if (list_empty (&adj_head[key]))
        {
//            printf ("没有该节点！\n");
            return NULL;
        }
    list_for_each_entry_safe(p, n, &adj_head[key],list)
        {
            /*加上某个条件后*/
            return p;
        }
    return NULL;
}

int
add_adj_table (struct adj_table *s)
{
    struct adj_table *p = NULL;
    struct adj_table *n;
    u32 key;
    key = get_adj_key (s->str_vrf, s->str_ip);
//    p = look_up_adj (s);
    if (p == NULL)
        {
            p = (struct adj_table *) malloc (sizeof(struct adj_table));
            if (p == NULL)
                {
                    return APP_ERR;
                }
            copy_to_adj (p, s);
            list_add_tail (&p->list, &adj_head[key]);
//            printf ("%s %s %s %d %s\n", p->str_vrf, p->str_ip, p->str_mac,
//                    p->int_vid, p->str_interface);
            return APP_SUCC;
        }
    else
        {
            copy_to_adj (p, s);
            printf ("adj重复值,不再添加!!!\n");
            return APP_SUCC;
        }
    return APP_ERR;
}

int
del_adj_table (const char *vrf, const char *ip)
{

    struct adj_table *p;
    struct adj_table *n;
    u32 key;
    key = get_adj_key (vrf, ip);
    if (list_empty (&adj_head[key]))
        {
//            printf ("没有该节点！\n");
            return APP_ERR;
        }
    list_for_each_entry_safe(p, n, &adj_head[key],list)
        {
            printf ("%s %s %s %d %s\n", p->str_vrf, p->str_ip, p->str_mac,
                    p->int_vid, p->str_interface);
            /*加上某个条件后*/
            list_del_init (&p->list);
            free (p);
        }
    return APP_SUCC;
}

int
free_adj_table (void)
{
    struct adj_table *p;
    struct adj_table *n;

    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            list_for_each_entry_safe(p,n,&adj_head[i],list)
                {
                    list_del_init (&p->list);
                    free (p);
                    p = NULL;
                }
        }
    return APP_SUCC;
}

u32
get_adj_key (const char *vrf, const char *ip)
{
    return (jhash_2words (jhash (vrf, strlen (vrf), HASH_INITVAL),
                          jhash (ip, strlen (ip), HASH_INITVAL), HASH_INITVAL)
            % HLIST_LEN_MAX);
}
int
copy_to_adj (struct adj_table *p, struct adj_table *s)
{

    strcpy (p->str_vrf, s->str_vrf);
    strcpy (p->str_ip, s->str_ip);
    p->int_vid = s->int_vid;
    strcpy (p->str_mac, s->str_mac);
    strcpy (p->str_interface, s->str_interface);
    return APP_SUCC;
}

/*
 *  结点有序插入adj链表
 */
#if 0
int
adj_add_sort (struct adj_table *new, struct list_head *head)
    {
        struct adj_table *padj;
        struct list_head *pos, *next;
        struct adj_table *temp;

        ASSERT (new);
        ASSERT (head);

        if (list_empty (head))
            {
                list_add_tail (&(new->list), head);
                return APP_SUCC;
            }

        temp = NULL;
        list_for_each_safe(pos, next, head)
            {
                padj = list_entry(pos, struct adj_table, list);
                if (!strcmp (padj->str_vrf, new->str_vrf)
                        && !strcmp (padj->str_ip, new->str_ip)
                        && !strcmp (padj->str_mac, new->str_mac)
                        && !strcmp (padj->str_vid, new->str_vid))
                    {
                        temp = padj;
                        /* 存在前4个成员与new一样，且第5个成员比new大的结点，new插在该结点前面 */
                        if (strcmp (padj->str_interface, new->str_interface) > 0)
                            {
                                list_add_tail (&(new->list), &(padj->list));
                                return APP_SUCC;
                            }
                    }
            }
        /* 存在前4个成员与new一样，但第5个成员都比new小，new插到这类结点的最后 */
        if (temp != NULL)
            {
                list_add (&(new->list), &(temp->list));
                return APP_SUCC;
            }

        temp = NULL;
        list_for_each_safe(pos, next, head)
            {
                padj = list_entry(pos, struct adj_table, list);
                if (!strcmp (padj->str_vrf, new->str_vrf)
                        && !strcmp (padj->str_ip, new->str_ip)
                        && !strcmp (padj->str_mac, new->str_mac))
                    {
                        temp = padj;
                        /* 存在前3个成员与new一样，且第4个成员比new大的结点，new插在该结点前面 */
                        if (padj->str_vid > new->str_vid)
                            {
                                list_add_tail (&(new->list), &(padj->list));
                                return APP_SUCC;
                            }
                    }
            }
        /* 存在前3个成员与new一样，但第4个成员都比new小，new插到这类结点的最后 */
        if (temp != NULL)
            {
                list_add (&(new->list), &(temp->list));
                return APP_SUCC;
            }

        temp = NULL;
        list_for_each_safe(pos, next, head)
            {
                padj = list_entry(pos, struct adj_table, list);
                if (!strcmp (padj->str_vrf, new->str_vrf)
                        && !strcmp (padj->str_ip, new->str_ip))
                    {
                        temp = padj;
                        /* 存在前2个成员与new一样，且第3个成员比new大的结点，new插在该结点前面 */
                        if (strcmp (padj->str_mac, new->str_mac) > 0)
                            {
                                list_add_tail (&(new->list), &(padj->list));
                                return APP_SUCC;
                            }
                    }
            }
        /* 存在前2个成员与new一样，但第3个成员都比new小，new插到这类结点的最后 */
        if (temp != NULL)
            {
                list_add (&(new->list), &(temp->list));
                return APP_SUCC;
            }

        temp = NULL;
        list_for_each_safe(pos, next, head)
            {
                padj = list_entry(pos, struct adj_table, list);
                if (!strcmp (padj->str_vrf, new->str_vrf))
                    {
                        temp = padj;
                        /* 存在前1个成员与new一样，且第2个成员比new大的结点，new插在该结点前面 */
                        if (ntohl (inet_addr (padj->str_ip))
                                > ntohl (inet_addr (new->str_ip)))
                            { /* str_ip要转成长整形比较 */
                                list_add_tail (&(new->list), &(padj->list));
                                return APP_SUCC;
                            }
                    }
            }
        /* 存在前1个成员与new一样，但第2个成员都比new小，new插到这类结点的最后 */
        if (temp != NULL)
            {
                list_add (&(new->list), &(temp->list));
                return APP_SUCC;
            }

        /* 没有任何成员与new一样，找到第1个成员比new大的结点，new插在该结点前面 */
        list_for_each_safe(pos, next, head)
            {
                padj = list_entry(pos, struct adj_table, list);
                if (strcmp (padj->str_vrf, new->str_vrf) > 0)
                    {
                        list_add_tail (&(new->list), &(padj->list));
                        return APP_SUCC;
                    }
            }
        /* new是最大的，直接插到尾部 */
        list_add_tail (&(new->list), head);

        return APP_SUCC;
    }
#endif
