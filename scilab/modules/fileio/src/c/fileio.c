/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "do_xxprintf.h"
#include "do_xxscanf.h"
#include "fileio.h"
#include "cvstr.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int NumTokens(char *string)
{
	char buf[128];
	int n      = 1;
	int lnchar = 0;
	int ntok   = -1;
	int length = (int)strlen(string)+1;
	
	if (string != 0)
	{
		/** Counting leading white spaces **/
		sscanf(string,"%*[ \r\t\n]%n",&lnchar);
		
		while ( n != 0 && n != EOF && lnchar <= length  )
		{
			int nchar1=0,nchar2=0;
			ntok++;
			n       = sscanf(&(string[lnchar]),"%[^ \r\t\n]%n%*[ \r\t\n]%n",buf,&nchar1,&nchar2);
			lnchar += (nchar2 <= nchar1) ? nchar1 : nchar2 ;
		}
		
		return(ntok);
	}
	return(FAIL);
}

/*--------------------------------------------------------------------------*/
/* Utility functions */
/*--------------------------------------------------------------------------*/
int StringConvert(char *str)
/* changes `\``n` --> `\n` idem for \t and \r */
{
  char *str1;
  int count=0;
  str1 = str;

  while ( *str != 0)
    {
      if ( *str == '\\' )
	{
	  switch ( *(str+1))
	    {
	    case 'n' : *str1 = '\n' ; str1++; str += 2;count++;break;
	    case 't' : *str1 = '\t' ; str1++; str += 2;break;
	    case 'r' : *str1 = '\r' ; str1++; str += 2;break;
	    default : *str1 = *str; str1++; str++;break;
	    }
	}
      else
	{
	  *str1 = *str; str1++; str++;
	}
    }
  *str1 = '\0';
  return count;
}
/*--------------------------------------------------------------------------*/
int Sci_Store(int nrow, int ncol, entry *data, sfdir *type, int retval_s)
{
  int cur_i,i,j,i1,one=1,zero=0,k,l,iarg,colcount;
  sfdir cur_type;
  char ** temp;


  /* create Scilab variable with each column of data */
  if (ncol+Rhs > intersiz ){
    Scierror(998,_("%s: Too many directives in scanf.\n"),"Sci_Store");
    return RET_BUG;
  }
  iarg=Rhs;
  if (Lhs > 1) {
    if (nrow==0) {/* eof encountered before any data */
      CreateVar(++iarg,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &l);
      LhsVar(1) = iarg;
      *stk(l) = -1.0;
      for ( i = 2; i <= Lhs ; i++)
      {
	iarg++;
	CreateVar(iarg,MATRIX_OF_DOUBLE_DATATYPE,&zero,&zero,&l);
	LhsVar(i) = iarg;
      }
      PutLhsVar();
      return 0;
    }
    CreateVar(++iarg,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &l);
    *stk(l) = (double) retval_s;
    LhsVar(1)=iarg;
    if (ncol==0) goto Complete;

    for ( i=0 ; i < ncol ; i++) {
      if ( (type[i] == SF_C) || (type[i] == SF_S) ) {
	if( (temp = (char **) MALLOC(nrow*ncol*sizeof(char **)))==NULL) return MEM_LACK;
	k=0;
	for (j=0;j<nrow;j++) temp[k++]=data[i+ncol*j].s;
	CreateVarFromPtr(++iarg,MATRIX_OF_STRING_DATATYPE, &nrow, &one, temp);
	FREE(temp);
	/*for (j=0;j<nrow;j++) FREE(data[i+ncol*j].s);*/
      }
      else {
	CreateVar(++iarg,MATRIX_OF_DOUBLE_DATATYPE, &nrow, &one, &l);
	for ( j=0 ; j < nrow ; j++)
	  *stk(l+j)= data[i+ncol*j].d;
      }

      LhsVar(i+2)=iarg;
    }
    /*FREE(data);*/
    /** we must complete the returned arguments up to Lhs **/
  Complete:
    for ( i = ncol+2; i <= Lhs ; i++)
      {
	iarg++;
	CreateVar(iarg,MATRIX_OF_DOUBLE_DATATYPE,&zero,&zero,&l);
	LhsVar(i) = iarg;
      }
  }
  else {/* Lhs==1 */
    char *ltype="cblock";
    int multi=0,endblk,ii,n;

    cur_type=type[0];

    for (i=0;i<ncol;i++)
      if (type[i] != cur_type) 	{
	multi=1;
	break;
      }
    if (multi) {
      i=(int)strlen(ltype);
      iarg=Rhs;
      CreateVarFromPtr(++iarg,STRING_DATATYPE, &one, &i, &ltype); /* the mlist type field */
      cur_type=type[0];
      i=0;cur_i=i;

      while (1) {
	if (i < ncol)
	  endblk=(type[i] != cur_type);
	else
	  endblk=1;
	if (endblk) {
	  colcount=i - cur_i;
	  if (nrow==0) {
	    CreateVar(++iarg,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &l);}
	  else if ( (cur_type == SF_C) || (cur_type == SF_S) ) {
	    if( (temp = (char **) MALLOC(nrow*colcount*sizeof(char **)))==NULL) return MEM_LACK;
	    k=0;
	    for (i1=cur_i;i1<i;i1++)
	      for (j=0;j<nrow;j++) temp[k++]=data[i1+ncol*j].s;
	    CreateVarFromPtr(++iarg,MATRIX_OF_STRING_DATATYPE, &nrow, &colcount,temp);
	    FREE(temp);
	    /*for (i1=cur_i;i1<i;i1++)
	      for (j=0;j<nrow;j++) FREE(data[i1+ncol*j].s);*/
	  }
	  else {
	    CreateVar(++iarg,MATRIX_OF_DOUBLE_DATATYPE, &nrow, &colcount, &l);
	    ii=0;
	    for (i1=cur_i;i1<i;i1++) {
	      for ( j=0 ; j < nrow ; j++)
		*stk(l+j+nrow*ii)= data[i1+ncol*j].d;
	      ii++;
	    }
	  }
	  if (i>=ncol) break;
	  cur_i=i;
	  cur_type=type[i];

	}
	i++;
      }
      n=iarg-Rhs; /* number of list fields*/

      iarg++;
      i=Rhs+1;
      C2F(mkmlistfromvars)(&i,&n);
      LhsVar(1)=i;
    }
    else {
      if (nrow==0) {
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &l);}
      else if ( (cur_type == SF_C) || (cur_type == SF_S) ) {
	if( (temp = (char **) MALLOC(nrow*ncol*sizeof(char **)))==NULL) return MEM_LACK;
	k=0;
	for (i1=0;i1<ncol;i1++)
	  for (j=0;j<nrow;j++) temp[k++]=data[i1+ncol*j].s;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &nrow, &ncol, temp);
	FREE(temp);
	/*for (i1=0;i1<ncol;i1++)
	  for (j=0;j<nrow;j++) FREE(data[i1+ncol*j].s);*/
      }
      else {
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &nrow, &ncol, &l);
	ii=0;
	for (i1=0;i1<ncol;i1++) {
	  for ( j=0 ; j < nrow ; j++)
	    *stk(l+j+nrow*ii)= data[i1+ncol*j].d;
	  ii++;
	}
      }
    }
    LhsVar(1)=Rhs+1;
  }
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
/* ************************************************************************
 *   Store data scanned by a single call to do_scan in line rowcount of data
 *   table
 ************************************************************************/
int Store_Scan(int *nrow, int *ncol, sfdir *type_s, sfdir *type, int *retval, int *retval_s, rec_entry *buf, entry **data, int rowcount, int n)
{
  int i,j,nr,nc,err;
  entry * Data;
  int blk=20; /* block size for memory allocation */
  nr=*nrow;
  nc=*ncol;

  if (rowcount==0) {
    for ( i=0 ; i < MAXSCAN ; i++) type_s[i]=SF_F; /* initialisation */
    if (nr<0) {
      nr=blk;
      *nrow=nr; /*added by S. Steer to fix bug 2453*/
    }
    nc=n;
    *ncol=nc;
    *retval_s=*retval;
    if (n==0) {
      return 0;
    }
    if ( (*data = (entry *) MALLOC(nc*nr*sizeof(entry)))==NULL) {
      err= MEM_LACK;
      goto bad1;
    }
    for ( i=0 ; i < nc ; i++) type_s[i]=type[i];

  }
  else {
    /* check if number of data read match with previous number */
    if ( (n !=nc ) || (*retval_s != *retval) ){
      err=MISMATCH;
      goto bad2;
    }
    /* check if types of data read match with previous types */
    for ( i=0 ; i < nc ; i++)
      if (type[i] != type_s[i]) {
	err=MISMATCH;
	goto bad2;
      }

    /* check for memory and REALLOC if necessary*/
    /*&&nc>0 added by S. Steer for bug 2399 fix */
    if (rowcount >= nr&& nc>0) {
      nr=nr+blk;
      *nrow=nr;
      if ( (*data = (entry *) REALLOC(*data,nc*nr*sizeof(entry)))==NULL) {
	err= MEM_LACK;
	goto bad2;
      }
    }
  }
  Data=*data;
  /* store values scanned in a new row */
  for ( i=0 ; i < nc ; i++)
    {
      switch ( type_s[i] )
	{
	case SF_C:
	case SF_S:
	  Data[i+nc*rowcount].s=buf[i].c;
	  break;
	case SF_LUI:
	  Data[i+nc*rowcount].d=(double)buf[i].lui;
	  break;
	case SF_SUI:
	  Data[i+nc*rowcount].d=(double)buf[i].sui;
	  break;
	case SF_UI:
	  Data[i+nc*rowcount].d=(double)buf[i].ui;
	  break;
	case SF_LI:
	  Data[i+nc*rowcount].d=(double)buf[i].li;
	  break;
	case SF_SI:
	  Data[i+nc*rowcount].d=(double)buf[i].si;
	  break;
	case SF_I:
	  Data[i+nc*rowcount].d=(double)buf[i].i;
	  break;
	case SF_LF:
	  Data[i+nc*rowcount].d=buf[i].lf;
	  break;
	case SF_F:
	  Data[i+nc*rowcount].d=(double)buf[i].f;
	  break;
	}
    } /* rowcount */
  return 0;
 bad1:
  /* FREE allocated strings in scan buffer */
  for ( j=0 ; j < MAXSCAN ; j++)
    if ( (type_s[j] ==  SF_C) || (type_s[j] ==  SF_S))  FREE(buf[j].c);

 bad2:
  return err;
}
/*--------------------------------------------------------------------------*/
void Free_Scan(int nrow, int ncol, sfdir *type_s, entry **data)
{
  int i,j;
  entry * Data;
  Data=*data;

  if (nrow != 0) {
    for ( j=0 ; j < ncol ; j++)
      if ( (type_s[j] ==  SF_C) || (type_s[j] ==  SF_S) )
	/* free allocated strings in scan data area */
	for ( i=0 ; i < nrow ; i++) {
	  FREE(Data[j+ncol*i].s);
	}
  }
  /* free scaned data area */
  if (ncol>0) FREE(Data);
}
/*--------------------------------------------------------------------------*/
/********************************************************
 * Converts a Scilab array of  String coded as integer array
 * into a regular string.
 * entries of the original array are catenated, separated by
 * '\n'   char
 ********************************************************/
int SciStrtoStr (int *Scistring, int *nstring, int *ptrstrings, char **strh)
{
  char *s,*p;
  int li,ni,*SciS,i,job=1;

  li=ptrstrings[0];
  ni=ptrstrings[*nstring] - li + *nstring +1;
  p=(char *) MALLOC(ni);
  if (p ==NULL)  return MEM_LACK;
  SciS= Scistring;
  s=p;
  for ( i=1 ; i<*nstring+1 ; i++)
    {
      ni=ptrstrings[i]-li;
      li=ptrstrings[i];
      F2C(cvstr)(&ni,SciS,s,&job,(long int)ni);
      SciS += ni;
      s += ni;
      if (i<*nstring) {
	*s='\n';
	s++;
      }
    }
  *s='\0';
  *strh=p;
  return 0;
}
/*--------------------------------------------------------------------------*/

