/* 
 * tools.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-15
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include "app.h"
#include "tools.h"
#include <stdio.h>
#include <string.h>

int conver_filename(char *infile, char *outfile) {
	char *token;
	char *outpath = outfile;
	if ((token = strsep(&infile, ".")) != NULL) {
		sprintf(outpath, "%s%s", token, "_result.txt");
	} else {
		return APP_ERR;
	}
	return APP_SUCC;
}
