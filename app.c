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

int
read_cmd (const char *path)
{
    char *line = NULL, *tmp_buf;
    size_t len;
    ssize_t read;
    char *name, *value;
    FILE *fp;
    fp = fopen (path, "r");
    if (fp == NULL)
        {
            return APP_ERR;
        }
    while ((read = getline(&line, &len, fp)) != -1)
      {
        tmp_buf = line;
        name = strsep(&tmp_buf, "=");
        if (name == NULL)
          continue;
        if (strcmp(name, "") == 0)
          continue;
        value = strsep(&tmp_buf, "\n");
        printf("%s ",value);
      }

    fclose (fp);
}

struct hlist_head arp_head[HLIST_LEN_MAX];
struct hlist_head mac_head[HLIST_LEN_MAX];
struct hlist_head adj_head[HLIST_LEN_MAX];

#if !DEBUG

int flg;

int
main (int argc, char **argv)
{

    u32 arp_key;
    u32 mac_key;
    u32 adj_key;

    struct arp_table *parp;
    struct mac_table *pmac;
    struct adj_table *padj;

    int i;
    for (i = 0; i < HLIST_LEN_MAX; ++i)
        {
            INIT_HLIST_HEAD(&arp_head[i]);

            INIT_HLIST_HEAD(&mac_head[i]);

            INIT_HLIST_HEAD(&adj_head[i]);
        }

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

    int nRet;

    while (!feof (infp))
        {

            nRet = fscanf (infp, "%s", cmd);
            if (nRet < 0)
                {
                    break;
                }
#if 1
            if (strcmp (cmd, "add-arp") == 0)
                {
                    parp = (struct arp_table *)malloc(sizeof(struct arp_table));
                    if(parp == NULL) {
                            retutn APP_ERR;
                    }
                    fscanf (infp, "%s%s%s%d", parp->str_vrf, parp->str_ip,
                            parp->str_mac, parp->int_vid);
                    arp_key = get_arp_key(parp->str_vrf,parp->str_ip);
                    add_arp_table (parp, &arp_head[arp_key]);
                    SET_FLAG(flg, ADD_ARP);
                }
            else if (strcmp (cmd, "add-mac") == 0)
                {
                    pmac = (struct mac_table *)malloc(sizeof(struct mac_table));
                        if(pmac == NULL) {
                                retutn APP_ERR;
                        }
                    fscanf (infp, "%d%s%s", pmac->ini_vid, pmac->str_mac,
                            pmac->str_interface);
                    mac_key = get_mac_key(pmac->ini_vid,pmac->str_mac);
                    add_mac_table (pmac, &mac_head[mac_key]);
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

            update_daj_node (outfp, &arp_head, &mac_head, &adj_head);

            if (CHECK_FLAG(flg,SHOW_ADJ_ALL) != 0
                    || CHECK_FLAG(flg,SHOW_ADJ) != 0)
                {
                    write_file (outfp, vrf, &adj_head);
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
    return APP_SUCC;
}

#endif

