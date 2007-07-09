#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "MALLOC.h" 
#include "src_string.h"


void convstr(char **Str, char **Str3, char typ, int mn)
{
	int x,y;
	for (x=0;x<mn;x++){
		for (y=0;y<strlen(Str[x]);y++){                 /*To traverse every string in the string matrix */
			if (typ=='u') {                             /*converts the matrix of strings  str-matrix  into upper case */
				if    ( Str[x][y]>=97 && Str[x][y]<=122 )  /* If it is the Lowercase letters */
            			Str3[x][y]=Str[x][y]-32		;
				else Str3[x][y]=Str[x][y];
			}
			else if (typ=='l') {                        /*converts the matrix of strings  str-matrix  into lower case */
				if    ( Str[x][y]>=65 && Str[x][y]<=90 )  /* if it is the uppercase letters*/
            			Str3[x][y]=Str[x][y]+32		;
                else Str3[x][y]=Str[x][y];
			}
		}
        Str3[x][y]=0;
	}
}