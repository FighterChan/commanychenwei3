/* 
 * t.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-22
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include <stdio.h>
int buf[] =
    { 1, 2, 3 };

int
fun (int *d)
{

    int i;
    for (i = 0; i < sizeof(buf)/sizeof(char); ++i)
        {
            printf ("%d\n", d[i]);
        }
    return 0;
}
int
main (int argc, char **argv)
{
    fun (buf);
    return 0;
}
