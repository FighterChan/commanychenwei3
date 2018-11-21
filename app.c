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

struct list_head arp_head;
struct list_head mac_head;
struct list_head adj_head;

#if !DEBUG

int flg;

int
main (int argc, char **argv)
{

    int i = 0;

    struct arp_table sarp;
    struct mac_table smac;

    memset (&sarp, 0, sizeof(struct arp_table));
    memset (&smac, 0, sizeof(struct mac_table));

    INIT_LIST_HEAD (&arp_head);

    INIT_LIST_HEAD (&mac_head);

    INIT_LIST_HEAD (&adj_head);

    char cmd[32];
    memset (cmd, 0, sizeof(cmd));

    char outpath[32];
    memset (outpath, 0, sizeof(outpath));

    char vrf[32 + 1];
    memset (vrf, 0, sizeof(vrf));

    CLEAR_FLAG_ALL(flg);

    FILE *infp;
    infp = fopen (argv[1], "r");
    ASSERT(infp);

    conver_filename (argv[1], outpath);

    FILE *outfp;
    outfp = fopen (outpath, "w");
    ASSERT(outfp);

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
                    fscanf (infp, "%s%s%s%s", sarp.str_vrf, sarp.str_ip,
                            sarp.str_mac, sarp.str_vid);
                    add_arp_table (&sarp, &arp_head);
                    SET_FLAG(flg, ADD_ARP);
                }
            else if (strcmp (cmd, "add-mac") == 0)
                {
                    fscanf (infp, "%s%s%s", smac.str_vid, smac.str_mac,
                            smac.str_interface);
                    add_mac_table (&smac, &mac_head);
                    SET_FLAG(flg, ADD_MAC);
                }
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
    free_arp_table (&arp_head);
    free_mac_table (&mac_head);
    free_adj_table (&adj_head);
    return APP_SUCC;
}
#endif

