/* 
 * app.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-13
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include "adj.h"
#include "arp.h"
#include "list.h"
#include "mac.h"
#include "app.h"
#include "tools.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if !DEBUG

int
arp_update_table (const char *vrf, const char *ip);
int
mac_update_table (int vid, const char *mac);
int
puts_adj (FILE *fp, char *strvrf);

int flg;

int
main (int argc, char **argv)
{

    struct arp_table sarp;
    struct mac_table smac;
    struct adj_table sadj;

    memset (&sarp, 0, sizeof(sarp));
    memset (&smac, 0, sizeof(smac));
    memset (&sadj, 0, sizeof(sadj));

    char cmd[32 + 1];
    memset (cmd, 0, sizeof(cmd));

    char vrf[32 + 1];
    memset (vrf, 0, sizeof(vrf));

    char outpath[32 + 1];
    memset (outpath, 0, sizeof(outpath));

    CLEAR_FLAG_ALL(flg);

    FILE *infp;
    infp = fopen (argv[1], "r");
    if (infp == NULL)
        {
            printf ("can not open input file!\n");
            return APP_ERR;
        }

    conver_filename (argv[1], outpath);

    FILE *outfp;
    outfp = fopen (outpath, "w");
    if (infp == NULL)
        {
            printf ("can not open output file!\n");
            fclose (infp);
            return APP_ERR;
        }

    init_arp_hash ();
    init_mac_hash ();
    init_adj_hash ();

    int nRet;

    while (!feof (infp))
        {

            nRet = fscanf (infp, "%s", cmd);
//            printf ("%s\n", cmd);
            if (nRet < 0)
                {
                    break;
                }
            if (strcmp (cmd, "add-arp") == 0)
                {
                    fscanf (infp, "%s%s%s%d", sarp.str_vrf, sarp.str_ip,
                            sarp.str_mac, &sarp.int_vid);
                    add_arp_table (&sarp);
                    arp_update_table (sarp.str_vrf, sarp.str_mac);
                    SET_FLAG(flg, ADD_ARP);
                }
            else if (strcmp (cmd, "add-mac") == 0)
                {
                    fscanf (infp, "%d%s%s", &smac.int_vid, smac.str_mac,
                            smac.str_interface);
                    add_mac_table (&smac);
                    mac_update_table (smac.int_vid, smac.str_mac);
                    SET_FLAG(flg, ADD_MAC);
                }

            else if (strcmp (cmd, "del-arp") == 0)
                {
                    fscanf (infp, "%s%s", sarp.str_vrf, sarp.str_ip);
                    del_arp_table (sarp.str_vrf,sarp.str_ip);
                    SET_FLAG(flg, DEL_ARP);
                }
            else if (strcmp (cmd, "del-mac") == 0)
                {
                    fscanf (infp, "%d%s", &smac.int_vid, smac.str_mac);
                    del_mac_table (smac.int_vid,smac.str_mac);
                    SET_FLAG(flg, DEL_MAC);
                }
            else if (strcmp (cmd, "del-vrf") == 0)
                {
                    fscanf (infp, "%s", sarp.str_vrf);
                    del_table_by_vrf (sarp.str_vrf);
                    SET_FLAG(flg, DEL_VRF);
                }
            else if (strcmp (cmd, "del-vid") == 0)
                {
                    fscanf (infp, "%d", &smac.int_vid);
                    del_table_by_vid (smac.int_vid);
                    SET_FLAG(flg, DEL_VID);
                }
            else if (strcmp (cmd, "show-adj-all") == 0)
                {
                    SET_FLAG(flg, SHOW_ADJ_ALL);
                    puts_adj (outfp, NULL);
                }
            else if (strcmp (cmd, "show-adj") == 0)
                {
                    fscanf (infp, "%s", vrf);
                    SET_FLAG(flg, SHOW_ADJ);
                    puts_adj (outfp, vrf);
                }
            else if (strcmp (cmd, "show-log") == 0)
                {
                    SET_FLAG(flg, SHOW_LOG);
                }

            CLEAR_FLAG(flg, SHOW_ADJ_ALL);
            memset (vrf, 0, sizeof(vrf));
            memset (cmd, 0, sizeof(cmd));
            memset (&sarp, 0, sizeof(struct arp_table));
            memset (&smac, 0, sizeof(struct mac_table));
#endif
        }

    fclose (infp);
    fclose (outfp);
    free_arp_table ();
    free_mac_table ();
    free_adj_table ();
    return APP_SUCC;
}

int
arp_update_table (const char *vrf, const char *ip)
{
    struct arp_table *parp;
    struct mac_table *pmac;
    struct adj_table sadj;
    memset (&sadj, 0, sizeof(struct adj_table));

    parp = look_up_arp (vrf, ip);
    if (parp == NULL)
        {
            return APP_ERR;
        }
    int key;
    key = get_key (parp->int_vid, parp->str_mac);
    if (list_empty (&mac_head[key]))
        {
            printf ("该mac链表为空!\n");
            return APP_ERR;
        }
    list_for_each_entry(pmac,&mac_head[key],list)
        {
            strcpy (sadj.str_vrf, parp->str_vrf);
            strcpy (sadj.str_ip, parp->str_ip);
            strcpy (sadj.str_mac, parp->str_mac);
            sadj.int_vid = parp->int_vid;
            strcpy (sadj.str_interface, pmac->str_interface);
            add_adj_table (&sadj);
            memset (&sadj, 0, sizeof(struct adj_table));
        }
    return APP_SUCC;
}

int
mac_update_table (int vid, const char *mac)
{
    struct arp_table *parp;
    struct mac_table *pmac;
    struct adj_table sadj;
    int arp_key = 0;
    memset (&sadj, 0, sizeof(struct adj_table));

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
                    list_for_each_entry(parp,&arp_head[i],list)
                        {
                            if (parp->int_vid == pmac->int_vid
                                    && strcmp (parp->str_mac, pmac->str_mac)
                                            == 0)
                                {
                                    strcpy (sadj.str_vrf, parp->str_vrf);
                                    strcpy (sadj.str_ip, parp->str_ip);
                                    strcpy (sadj.str_mac, parp->str_mac);
                                    sadj.int_vid = parp->int_vid;
                                    strcpy (sadj.str_interface,
                                            pmac->str_interface);
                                    add_adj_table (&sadj);
                                    memset (&sadj, 0, sizeof(struct adj_table));
                                }

                        }
                }
        }

    return APP_SUCC;
}

int
del_table_by_vid (int vid)
{

    struct mac_table *p, *n;
    struct adj_table *padj, *nadj;
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {

            if (!list_empty (&mac_head[i]))
                {
                    list_for_each_entry_safe(p, n, &mac_head[i],list)
                        {
                            /*加上某个条件后*/
                            if (p->int_vid == vid)
                                {
//                                    printf ("mac中删除vid = %d\n", p->int_vid);
                                    list_del_init (&p->list);
                                    free (p);
                                    p = NULL;
                                }
                        }
                }

            if (!list_empty (&adj_head[i]))
                {
                    list_for_each_entry_safe(padj, nadj, &adj_head[i],list)
                        {
                            /*加上某个条件后*/
                            if (padj->int_vid == vid)
                                {
//                                    printf ("adj中删除vid = %d\n", padj->int_vid);
                                    list_del_init (&padj->list);
                                    free (padj);
                                    padj = NULL;
                                }
                        }
                }

        }
    return APP_SUCC;
}

int
del_table_by_vrf (const char *vrf)
{

    struct arp_table *p, *n;
    struct adj_table *padj, *nadj;
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
//                                    printf ("arp中刪除%s\n", p->str_vrf);
                                    list_del_init (&p->list);
                                    free (p);
                                    p = NULL;
                                }
                        }
                }
            if (!list_empty (&adj_head[i]))
                {
                    list_for_each_entry_safe(padj, nadj, &adj_head[i],list)
                        {
                            /*加上某个条件后*/
                            if (strcmp (padj->str_vrf, vrf) == 0)
                                {
//                                    printf ("adj中刪除%s\n", padj->str_vrf);
                                    list_del_init (&padj->list);
                                    free (padj);
                                    padj = NULL;
                                }
                        }
                }

        }
    return APP_SUCC;
}

int
puts_adj (FILE *fp, char *strvrf)
{
    struct adj_table *padj, *nadj;
    int adj_count;
    adj_count = 0;
    int i;
    char vrf[32 + 1];
    memset (vrf, 0, sizeof(vrf));
    if (vrf != NULL)
        {
            conver_vrfname (strvrf, vrf);
        }
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            if (!list_empty (&adj_head[i]))
                {
                    list_for_each_entry_safe(padj,nadj,&adj_head[i],list)
                        {
                            if ((CHECK_FLAG(flg,SHOW_ADJ) != 0 && vrf != NULL
                                    && strcmp (padj->str_vrf, vrf) == 0)
                                    || (CHECK_FLAG(flg,SHOW_ADJ_ALL) != 0))
                                {
                                    adj_count++;
                                }
                        }
                }
        }

    fprintf (fp, "count:%d\n", adj_count);

    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            if (!list_empty (&adj_head[i]))
                {
                    list_for_each_entry_safe(padj,nadj,&adj_head[i],list)
                        {
                            if ((CHECK_FLAG(flg,SHOW_ADJ) != 0 && vrf != NULL
                                    && strcmp (padj->str_vrf, vrf) == 0)
                                    || (CHECK_FLAG(flg,SHOW_ADJ_ALL) != 0))
                                {

                                    fprintf (fp, "%s %s %s %d %s\n",
                                             padj->str_vrf, padj->str_ip,
                                             padj->str_mac, padj->int_vid,
                                             padj->str_interface);
                                }
                        }
                }
        }

    return APP_SUCC;
}

u32
get_key (u32 vid, const char *mac)
{
    return (jhash_2words (vid, jhash (mac, strlen (mac), HASH_INITVAL),
    HASH_INITVAL) % HLIST_LEN_MAX);
}
