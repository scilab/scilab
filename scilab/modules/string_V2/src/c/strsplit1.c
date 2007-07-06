#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "MALLOC.h" 

void strsplit1(char **Str,char **Str3,int *u,int *v,int *w,int mn,int *m4)
{
	int x;
	for (x=0;x<mn;x++){
		if (x!=m4[*v]) Str3[*u][(*w)++]=Str[0][x];
		else {
			(*u)++;
			(*v)++;
			*w=0;
		}
        Str3[*u][*w]=0;
	}
}