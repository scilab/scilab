#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "MALLOC.h" 
void tokens(char **Str,char **Str2,char **Str3,int *u,int *w,int mn,int mn2)
{
	int x,y,bo,i;
	for (x=0;x<mn;x++){
		for (y=0;y<strlen(Str[x]);y++){
				for (i=0; i<mn2; i++)  if (Str[x][y]==Str2[i][0]) bo=1;
				if    (bo==0) Str3[*u][(*w)++]=Str[0][y];
				else {
					Str3[*u][*w]=0;
					(*u)++;
					*w=0;
				}
				bo=0;
		}
	}
}