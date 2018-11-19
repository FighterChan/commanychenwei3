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

	ASSERT(infile);
	ASSERT(outfile);

	char *token;
	char *outpath = outfile;
	if ((token = strsep(&infile, ".")) != NULL) {
		sprintf(outpath, "%s%s", token, "_result.txt");
	} else {
		return APP_ERR;
	}
	return APP_SUCC;
}

int conver_vrfname(char *s, char *name) {

	ASSERT(s);
	ASSERT(name);

	char *token;
	char *outname = name;
	int index = 0;
	while((token = strsep(&s,"=")) != NULL) {
		if(index == 1) {
			strcpy(outname,token);
		}
		index++;
	}
	return APP_SUCC;
}
