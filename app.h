/* /chenwei3/app.h */
#ifndef _APP_H_
#define _APP_H_

#define DEBUG (0)

#define APP_SUCC (0)
#define APP_ERR  (-1)

/* 各个链表结构字段长度定义 */
#define VRF_LEN_MAX    (32 + 1)
#define IP_LEN_MAX     (16 + 1)
#define MAC_LEN_MAX    (16 + 1)
#define VID_LEN_MAX    (4094)
#define VID_LEN_MIN    (1)
#define INTF_LEN_MAX   (32 + 1)

/*链表数组最大长度*/
#define HLIST_LEN_MAX  (1024)

extern int flg;
/*各个命令标志位*/
enum
{
    ADD_ARP = (unsigned long) (1 << 0),
    ADD_MAC = (unsigned long) (1 << 1),
    DEL_ARP = (unsigned long) (1 << 2),
    DEL_MAC = (unsigned long) (1 << 3),
    DEL_VRF = (unsigned long) (1 << 4),
    DEL_VID = (unsigned long) (1 << 5),
    SHOW_ADJ_ALL = (unsigned long) (1 << 6),
    SHOW_ADJ = (unsigned long) (1 << 7),
    SHOW_LOG = (unsigned long) (1 << 8)
};

#define SET_FLAG(x,y)		(x |= y)
#define CHECK_FLAG(x,y)		(x & y)
#define CHECK_FLAG_ALL(x)	(x != 0)
#define CLEAR_FLAG(x,y)		(x &= ~(y))
#define CLEAR_FLAG_ALL(x)	(x &= 0)

#define CMD_MAX (9)
struct exe_cmd {
    char *cmd;
    char *line;
    int type;
};

/* body of app.h */
#endif /* _APP_H_ */
