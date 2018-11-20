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

/*
 *  结点有序插入adj链表
 */
int
adj_add_sort (struct adj_table *new, struct list_head *head)
{
  struct adj_table *padj;
  struct list_head *pos, *next;
  struct adj_table *temp;

  ASSERT(new);
  ASSERT(head);

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

int
add_adj_table (FILE *fp, struct adj_table *s, struct list_head *head)
{

  struct adj_table *p;
  struct list_head *pos, *next;

  ASSERT(s);
  ASSERT(head);

#if 1
  list_for_each_safe(pos,next,head)
    {
      p = list_entry(pos, struct adj_table, list);
      if (p && strcmp (p->str_ip, s->str_ip) == 0
	  && strcmp (p->str_vrf, s->str_vrf) == 0
	  && strcmp (p->str_interface, s->str_interface) == 0
	  && strcmp (p->str_mac, s->str_mac) == 0
	  && strcmp (p->str_vid, s->str_vid) == 0)
	{

	  return APP_ERR;
	}
    }
#endif

  MALLOC(struct adj_table, p);

  strcpy (p->str_vid, s->str_vid);
  strcpy (p->str_interface, s->str_interface);
  strcpy (p->str_mac, s->str_mac);
  strcpy (p->str_ip, s->str_ip);
  strcpy (p->str_vrf, s->str_vrf);
  p->counter = 0;
  if (CHECK_FLAG(flg,SHOW_LOG) != 0)
    {
      fprintf (fp, "%s %s %s %s %s %s\n", "add-adj", p->str_vrf, p->str_ip,
	       p->str_mac, p->str_vid, p->str_interface);
    }
//	list_add_tail(&p->list, head);
  adj_add_sort (p, head);
  return APP_SUCC;
}

int
del_adj_table (FILE *fp, struct list_head *head)
{

  struct adj_table *p;
  struct list_head *pos, *next;

  ASSERT(fp);
  ASSERT(head);

  list_for_each_safe(pos,next,head)
    {
      p = list_entry(pos, struct adj_table, list);
      if (p->counter == 1)
	{
	  if (CHECK_FLAG(flg,SHOW_LOG) != 0)
	    {
	      fprintf (fp, "%s %s %s %s %s %s\n", "del-adj", p->str_vrf,
		       p->str_ip, p->str_mac, p->str_vid, p->str_interface);
	    }
	  list_del_init (pos);
	  free (p);
	}
    }

  return APP_SUCC;
}

int
del_table_by_vrf (FILE *fp, struct arp_table *s, struct list_head *arp_head,
		  struct list_head *adj_head)
{

  struct list_head *pos, *next;

  struct arp_table *parp;

  struct adj_table *padj;

  ASSERT(fp);
  ASSERT(s);
  ASSERT(arp_head);
  ASSERT(adj_head);

#if 0
  /*删除邻接表*/
  list_for_each_safe(pos,next,adj_head)
    {
      padj = list_entry(pos, struct adj_table, list);
      if (strcmp(padj->str_vrf, s->str_vrf) == 0)
	{
	  /*标志老化节点*/
	  padj->counter--;
	  if (CHECK_FLAG(flg,SHOW_LOG) != 0)
	    {
	      fprintf(fp, "%s %s %s %s %s %s\n", "del-adj", padj->str_vrf,
		  padj->str_ip, padj->str_mac, padj->str_vid,
		  padj->str_interface);
	    }
	  list_del_init(pos);
	  free(padj);
	}
    }
#endif
  /*删除arp表*/
  list_for_each_safe(pos,next,arp_head)
    {
      parp = list_entry(pos, struct arp_table, list);
      if (strcmp (parp->str_vrf, s->str_vrf) == 0)
	{
	  list_del_init (pos);
	  free (parp);
	}
    }
  return APP_SUCC;
}

int
del_table_by_vid (FILE *fp, struct mac_table *s, struct list_head *mac_head,
		  struct list_head *adj_head)
{

  struct list_head *pos, *next;

  struct mac_table *pmac;

  struct adj_table *padj;

  ASSERT(fp);
  ASSERT(s);
  ASSERT(mac_head);
  ASSERT(adj_head);

#if 0
  /*删除邻接表*/
  list_for_each_safe(pos,next,adj_head)
    {
      padj = list_entry(pos, struct adj_table, list);
      if (strcmp(padj->str_vid, s->str_vid) == 0)
	{
	  /*标志老化节点*/
	  padj->counter--;
	  if (CHECK_FLAG(flg,SHOW_LOG) != 0)
	    {
	      fprintf(fp, "%s %s %s %s %s %s\n", "del-adj", padj->str_vrf,
		  padj->str_ip, padj->str_mac, padj->str_vid,
		  padj->str_interface);
	    }
	  list_del_init(pos);
	  free(padj);
	}
    }
#endif
  /*删除mac表*/
  list_for_each_safe(pos,next,mac_head)
    {
      pmac = list_entry(pos, struct mac_table, list);
      if (strcmp (pmac->str_vid, s->str_vid) == 0)
	{
	  list_del_init (pos);
	  free (pmac);
	}
    }

  return APP_SUCC;
}

int
free_adj_table (struct list_head *head)
{

  struct list_head *pos, *next;
  struct adj_table *p;

  ASSERT(head);

  list_for_each_safe(pos,next,head)
    {
      p = list_entry(pos, struct adj_table, list);
      list_del_init (pos);
      free (p);
    }
  return APP_SUCC;
}

int
write_file (FILE *outfp, char *vrf, struct list_head *head)
{

  struct list_head *pos, *next;
  struct adj_table *p;
  int index;
  char vrf_name[32];

  ASSERT(vrf);
  ASSERT(outfp);
  ASSERT(head);

  memset (vrf_name, 0, sizeof(vrf_name));

  conver_vrfname (vrf, vrf_name);

  /*数出总的adj表项条数*/
  index = 0;
  list_for_each_safe(pos,next,head)
    {
      p = list_entry(pos, struct adj_table, list);
      if (CHECK_FLAG(flg,SHOW_ADJ) != 0)
	{
	  if (strcmp (p->str_vrf, vrf_name) == 0)
	    {
	      index++;
	    }
	}
      else
	{
	  index++;
	}
    }

  /*adj表项条数、写入文件*/
  fprintf (outfp, "count:%d\n", index);

  /*写入adj表项*/
  list_for_each_safe(pos,next,head)
    {
      p = list_entry(pos, struct adj_table, list);

      if (CHECK_FLAG(flg,SHOW_ADJ) != 0)
	{
	  if (strcmp (p->str_vrf, vrf_name) == 0)
	    {
	      fprintf (outfp, "%s %s %s %s %s\n", p->str_vrf, p->str_ip,
		       p->str_mac, p->str_vid, p->str_interface);
	    }
	}
      else
	{
	  fprintf (outfp, "%s %s %s %s %s\n", p->str_vrf, p->str_ip, p->str_mac,
		   p->str_vid, p->str_interface);
	}
    }
  return APP_SUCC;
}

/*
 * 	新增加的表项跟已存在的表项一样，则去除老化标记，防止新表项被删除
 * */

int
look_up_adj (struct adj_table *s, struct list_head *head)
{
  struct adj_table *padj;
  struct list_head *pos, *next;

  ASSERT(s);
  ASSERT(head);

  list_for_each_safe(pos, next, head)
    {
      padj = list_entry(pos, struct adj_table, list);

      if (strcmp (padj->str_vrf, s->str_vrf) == 0
	  && strcmp (padj->str_ip, s->str_ip) == 0
	  && strcmp (padj->str_mac, s->str_mac) == 0
	  && strcmp (padj->str_vid, s->str_vid) == 0
	  && strcmp (padj->str_interface, s->str_interface) == 0)
	{

	  padj->counter = 0;
	}
    }

  return APP_SUCC;
}

/*
 * 	动态更新adj表项
 * */
int
update_daj_node (FILE *fp, struct list_head *sarp_head,
		 struct list_head *smac_head, struct list_head *sadj_head)
{

  struct list_head *arp_pos, *arp_next;
  struct list_head *mac_pos, *mac_next;
  struct list_head *adj_pos, *adj_next;
  struct arp_table *parp_t;
  struct mac_table *pmac_t;
  struct adj_table *padj_t;
  struct adj_table sadj;

  ASSERT(fp);
  ASSERT(sarp_head);
  ASSERT(smac_head);
  ASSERT(sadj_head);

  memset (&sadj, 0, sizeof(struct adj_table));

  /*1.给adj节点设置老化标志*/
  list_for_each_safe(adj_pos,adj_next,sadj_head)
    {
      padj_t = list_entry(adj_pos, struct adj_table, list);
      padj_t->counter = 1;
    }

  /*2.查询所有新增的adj表项,如果有跟旧表项重复，则去除老化标记；*/
  list_for_each_safe(arp_pos,arp_next,sarp_head)
    {
      parp_t = list_entry(arp_pos, struct arp_table, list);
      list_for_each_safe(mac_pos,mac_next,smac_head)
	{
	  pmac_t = list_entry(mac_pos, struct mac_table, list);

	  if (strcmp (parp_t->str_mac, pmac_t->str_mac) == 0
	      && strcmp (parp_t->str_vid, pmac_t->str_vid) == 0)
	    {

	      strcpy (sadj.str_interface, pmac_t->str_interface);
	      strcpy (sadj.str_ip, parp_t->str_ip);
	      strcpy (sadj.str_vrf, parp_t->str_vrf);
	      strcpy (sadj.str_mac, parp_t->str_mac);
	      strcpy (sadj.str_vid, parp_t->str_vid);

	      /*所有新旧节点*/
	      look_up_adj (&sadj, sadj_head);
	    }
	}
    }

  /*3.删除老化节点；*/
  del_adj_table (fp, sadj_head);

  memset (&sadj, 0, sizeof(struct adj_table));

  /*4.添加新节点;*/
  list_for_each_safe(arp_pos,arp_next,sarp_head)
    {
      parp_t = list_entry(arp_pos, struct arp_table, list);
      list_for_each_safe(mac_pos,mac_next,smac_head)
	{
	  pmac_t = list_entry(mac_pos, struct mac_table, list);

	  if (strcmp (parp_t->str_mac, pmac_t->str_mac) == 0
	      && strcmp (parp_t->str_vid, pmac_t->str_vid) == 0)
	    {
	      strcpy (sadj.str_interface, pmac_t->str_interface);
	      strcpy (sadj.str_ip, parp_t->str_ip);
	      strcpy (sadj.str_vrf, parp_t->str_vrf);
	      strcpy (sadj.str_mac, parp_t->str_mac);
	      strcpy (sadj.str_vid, parp_t->str_vid);
	      add_adj_table (fp, &sadj, sadj_head);
	    }
	}
    }

  return APP_SUCC;
}

