/*------------------------------------------------------------------------
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../machine.h"

C2F(ctest)(str,lstr,ilstr)
     char str[];
     int *lstr;
     long int ilstr;
{
  int i;
  fprintf(stderr,"The string transmitted [");
  for (i=0;i<*lstr;i++)
      fprintf(stderr,"%c",str[i]);
  fprintf(stderr,"]\n");
  strncpy(str,"A returned\n string\n",*lstr);
  fprintf(stderr,"and now [%s],%d\n",str,strlen(str));
  *lstr=strlen(str);
}


C2F(cvs2c)(n,line,str,csiz,alfa,alfb,lstr,lalfa,lalfb)
     int *n,line[],*csiz;
     char str[],alfa[],alfb[];
     long int lstr,lalfa,lalfb;
{
  int eol=99,j;
  for (j=0; j < *n; j++)
    {
      int m=line[j];
      if ( m > *csiz || m < - *csiz) m=eol;
      if ( m == eol )
	str[j]= '\n';
      else
	str[j]= ( m < 0) ? alfb[-m] : alfa[m];
    }
}

C2F(cvc2s)(n,line,str,csiz,alfa,alfb,lstr,lalfa,lalfb)
     int *n,line[],*csiz;
     char str[],alfa[],alfb[];
     long int lstr,lalfa,lalfb;
{
  int eol=99,j ;
  for (j = *n-1; j >= 0 ; j--)
    {
      int k,flag=0;
      char mc=str[j];
      for ( k =0 ; k < *csiz; k++)
	{
	  if (mc == alfa[k] ) {
	    line[j]=k; flag=1;break;}
	  else  
	    if ( mc == alfb[k] ) 
	      {
		line[j] = -(k); flag=1;break;}
	}
      if (flag==0) line[j]= eol;
    }
}







