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
update_table (struct arp_table *arp, struct mac_table *mac);
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
                    SET_FLAG(flg, ADD_ARP);
                }
            else if (strcmp (cmd, "add-mac") == 0)
                {
                    fscanf (infp, "%d%s%s", &smac.int_vid, smac.str_mac,
                            smac.str_interface);
                    add_mac_table (&smac);
                    SET_FLAG(flg, ADD_MAC);
                }
#if 0
            else if (strcmp (cmd, "del-arp") == 0)
                {
                    fscanf (infp, "%s%s", sarp.str_vrf, sarp.str_ip);
                    del_arp_table (&sarp, &arp_head);
                    SET_FLAG(flg, DEL_ARP);
                }
            else if (strcmp (cmd, "del-mac") == 0)
                {
                    fscanf (infp, "%s%s", smac.str_vid, smac.str_mac);
                    del_mac_table (&smac, &mac_head);
                    SET_FLAG(flg, DEL_MAC);
                }
            else if (strcmp (cmd, "del-vrf") == 0)
                {
                    fscanf (infp, "%s", sarp.str_vrf);
                    del_table_by_vrf (outfp, &sarp, &arp_head, &adj_head);
                    SET_FLAG(flg, DEL_VRF);
                }
            else if (strcmp (cmd, "del-vid") == 0)
                {
                    fscanf (infp, "%s", smac.str_vid);
                    del_table_by_vid (outfp, &smac, &mac_head, &adj_head);
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

            update_table (&sarp, &smac);

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
update_table (struct arp_table *arp, struct mac_table *mac)
{
    struct arp_table *parp;
    struct hlist_node *narp;
    struct mac_table *pmac;
    struct arp_table sarp;
    struct mac_table smac;
    struct adj_table sadj;
    memset (&sarp, 0, sizeof(struct arp_table));
    memset (&smac, 0, sizeof(struct mac_table));
    memset (&sadj, 0, sizeof(struct adj_table));
    int i;
    for (int i = 0; i < HLIST_LEN_MAX; ++i)
        {
            hlist_for_each_entry_safe(parp,narp,&arp_head[i],list)
                {
                    if (hlist_empty (&arp_head[i]))
                        {

                        }
                    else
                        {
                            smac.int_vid = parp->int_vid;
                            strcpy (smac.str_mac, parp->str_mac);
                            pmac = look_up_mac (&smac);
                            memset (&smac, 0, sizeof(struct mac_table));
                            if (pmac != NULL)
                                {
//                                    printf ("查找mac成功\n");
                                    strcpy (sadj.str_vrf, parp->str_vrf);
                                    strcpy (sadj.str_ip, parp->str_ip);
                                    strcpy (sadj.str_mac, parp->str_mac);
                                    sadj.int_vid = parp->int_vid;
                                    strcpy (sadj.str_interface,
                                            pmac->str_interface);

//                                    printf ("%s %s %s %d %s\n", sadj.str_vrf,
//                                            sadj.str_ip, sadj.str_mac,
//                                            sadj.int_vid, sadj.str_interface);
                                    add_adj_table (&sadj);
                                    memset (&sadj, 0, sizeof(struct adj_table));
                                }
                        }
                }
        }

//    parp = look_up_arp (arp);
//    if (parp != NULL)
//        {
//
//            printf ("查找arp成功\n");
//            smac.int_vid = parp->int_vid;
//            strcpy (smac.str_mac, parp->str_mac);
//            pmac = look_up_mac (&smac);
//            if (pmac == NULL)
//                {
//                    printf ("查找mac失败\n");
//                    return APP_ERR;
//                }
//        }
//    else
//        {
//            pmac = look_up_mac (mac);
//            if (pmac == NULL)
//                {
//                    printf ("查找mac失败\n");
//                    return APP_ERR;
//                }
//
//        }

    return APP_SUCC;
}

int
puts_adj (FILE *fp)
{
    struct adj_table *padj;
    struct hlist_node *nadj;
    int i;
    for (int i = 0; i < HLIST_LEN_MAX; ++i)
        {
            hlist_for_each_entry_safe(padj,nadj,&adj_head[i],list)
                {
                    if (hlist_empty (&adj_head[i]))
                        {

                        }
                    else
                        {
                            fprintf (fp, "%s %s %s %d %s\n", padj->str_vrf,
                                     padj->str_ip, padj->str_mac, padj->int_vid,
                                     padj->str_interface);
                        }
                }
        }
    return APP_SUCC;
}

