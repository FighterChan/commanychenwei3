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
arp_update_table (struct arp_table *arp);
int
mac_update_table (struct mac_table *mac);
int
puts_adj (FILE *fp);

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

    char cmd[32];
    memset (cmd, 0, sizeof(cmd));

    char outpath[32];
    memset (outpath, 0, sizeof(outpath));

    char vrf[32 + 1];
    memset (vrf, 0, sizeof(vrf));

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
                    arp_update_table (&sarp);
                    SET_FLAG(flg, ADD_ARP);
                }
            else if (strcmp (cmd, "add-mac") == 0)
                {
                    fscanf (infp, "%d%s%s", &smac.int_vid, smac.str_mac,
                            smac.str_interface);
                    add_mac_table (&smac);
                    mac_update_table (&smac);
                    SET_FLAG(flg, ADD_MAC);
                }

            else if (strcmp (cmd, "del-arp") == 0)
                {
                    fscanf (infp, "%s%s", sarp.str_vrf, sarp.str_ip);
                    del_arp_table (&sarp);
                    arp_update_table (&sarp);
                    SET_FLAG(flg, DEL_ARP);
                }
            else if (strcmp (cmd, "del-mac") == 0)
                {
                    fscanf (infp, "%d%s", &smac.int_vid, smac.str_mac);
                    del_mac_table (&smac);
                    mac_update_table (&smac);
                    SET_FLAG(flg, DEL_MAC);
                }
#if 1
            else if (strcmp (cmd, "del-vrf") == 0)
                {
                    fscanf (infp, "%s", sarp.str_vrf);
                    del_table_by_vrf (sarp.str_vrf);
                    arp_update_table (&sarp);
                    SET_FLAG(flg, DEL_VRF);
                }
            else if (strcmp (cmd, "del-vid") == 0)
                {
                    fscanf (infp, "%d", &smac.int_vid);
                    del_table_by_vid (smac.int_vid);
                    mac_update_table (&smac);
                    SET_FLAG(flg, DEL_VID);
                }
#endif
            else if (strcmp (cmd, "show-adj-all") == 0)
                {
                    SET_FLAG(flg, SHOW_ADJ_ALL);
                }
            else if (strcmp (cmd, "show-adj") == 0)
                {
                    fscanf (infp, "%s", vrf);
                    SET_FLAG(flg, SHOW_ADJ);
                }
            else if (strcmp (cmd, "show-log") == 0)
                {
                    SET_FLAG(flg, SHOW_LOG);
                }

//            update_table (&sarp, &smac);

            if (CHECK_FLAG(flg,SHOW_ADJ_ALL) != 0
                    || CHECK_FLAG(flg,SHOW_ADJ) != 0)
                {
                    puts_adj (outfp);
                    memset (vrf, 0, sizeof(vrf));
                }

            CLEAR_FLAG(flg, SHOW_ADJ_ALL);
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
arp_update_table (struct arp_table *arp)
{
    struct arp_table *parp;
    struct arp_table *narp;
    struct mac_table *pmac;
    struct mac_table *nmac;
    struct adj_table sadj;
    int mac_key;
    memset (&sadj, 0, sizeof(struct adj_table));

    parp = look_up_arp (arp);
    if (parp == NULL)
        {
            return APP_ERR;
        }

    mac_key = get_mac_key (parp->int_vid, parp->str_mac);

    list_for_each_entry_safe(pmac,nmac,&mac_head[mac_key],list)
        {

                {
                    strcpy (sadj.str_vrf, parp->str_vrf);
                    strcpy (sadj.str_ip, parp->str_ip);
                    strcpy (sadj.str_mac, parp->str_mac);
                    sadj.int_vid = parp->int_vid;
                    strcpy (sadj.str_interface, pmac->str_interface);
                    add_adj_table (&sadj);
                    memset (&sadj, 0, sizeof(struct adj_table));
                }
        }

    return APP_SUCC;
}

int
mac_update_table (struct mac_table *mac)
{
    struct arp_table *parp;
    struct arp_table *narp;
    struct mac_table *pmac;
    struct mac_table *nmac;
    struct adj_table sadj;
    int arp_key;
    memset (&sadj, 0, sizeof(struct adj_table));

    pmac = look_up_mac (mac);
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

                            if (pmac->int_vid == parp->int_vid
                                    && strcmp (pmac->str_mac, parp->str_mac)
                                            == 0)
//                    arp_key = get_arp_key (parp->str_vrf, parp->str_ip);
//                    if (i == arp_key)
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
                                    printf ("mac中删除vid = %d\n", p->int_vid);
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
                                    printf ("adj中删除vid = %d\n", padj->int_vid);
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
                                    printf ("arp中刪除%s\n", p->str_vrf);
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
                                    printf ("adj中刪除%s\n", padj->str_vrf);
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
puts_adj (FILE *fp)
{
    struct adj_table *padj, *nadj;
    int adj_count;
    adj_count = 0;
    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            if (!list_empty (&adj_head[i]))
                {
                    list_for_each_entry_safe(padj,nadj,&adj_head[i],list)
                        {
                            adj_count++;
                        }
                }
        }

    fprintf(fp,"count:%d\n",adj_count);

    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            if (!list_empty (&adj_head[i]))
                {
                    list_for_each_entry_safe(padj,nadj,&adj_head[i],list)
                        {

                            fprintf (fp, "%s %s %s %d %s\n", padj->str_vrf,
                                     padj->str_ip, padj->str_mac, padj->int_vid,
                                     padj->str_interface);
                        }
                }
        }

    return APP_SUCC;
}
