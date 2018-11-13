/* /adj_table/main.h */
#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>

enum {
	FALSE, TRUE = !FALSE
};

enum {
	SHOW_SELECT, SHOW_ALL,
};

#define APP_SUCC (0)
#define APP_ERR  (-1)

#define DEFINE_PARAM(x)\
do { \
	if (!x) { \
	printf("%s error!\n",x); \
	return APP_ERR; \
	} \
}while(0)

#define CMD_MAX (9)

/* body of main.h */
#endif /* _MAIN_H_ */
