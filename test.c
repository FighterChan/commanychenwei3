/* 
 * main.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-15
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "app.h"
#include "jhash.h"

#if !DEBUG
#else
struct node
    {
        char name[32];
        char sex[8];
        int year;
        struct list_head list;
    };

struct list_head shead[1024];

int list_test(struct list_head *head)
    {

        struct node *pnode;

        int i;
        for (i = 0; i < 1024; ++i)
            {
                pnode = (struct node *)malloc(sizeof(struct node));
                if(!pnode)
                    {
                        return -1;
                    }
                sprintf(pnode->name,"name:%d",i);
                sprintf(pnode->sex,"sex:%d",i);
                list_add_tail(&pnode->list,head);
            }
        return 0;
    }

int for_each(struct list_head *head)
    {
        struct node *pnode;
        struct list_head *pos;
        list_for_each(pos,head)
            {
                pnode = list_entry(pos,struct node,list);
                printf("%s\n",pnode->name);
            }
        return 0;
    }

int for_each_save(struct list_head *head)
    {
        struct node *pnode;
        struct list_head *pos,*next;

        list_for_each_safe(pos,next,head)
            {
                pnode = list_entry(pos,struct node,list);
                list_del_init(pos);
                free(pnode);
            }
        return 0;
    }

int main(int argc, char **argv)
    {

        INIT_LIST_HEAD(&shead);
        list_test(&shead);
        for_each(&shead);
        for_each_save(&shead);
        return 0;
    }
#endif
