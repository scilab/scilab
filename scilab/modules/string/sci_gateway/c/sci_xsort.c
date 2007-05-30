/*------------------------------------------------------------------------*/
/* file: sci_xsort.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xsort routine                                     */
/*------------------------------------------------------------------------*/
#include "gw_string.h"
#include "stack-c.h"
#include "gsort.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_xsort) _PARAMS((char *fname, unsigned long fname_len))
{
  char **S;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3/* ,v */;
  integer un=1,lex,iflag = 0;
  char iord[2] ; /* = { 'd','\0' }; */
  char typex[10]; /* = { 'g' ,'\0'} ; */
  SciIntMat Im;
  int Type;
  iord[0] = 'd'; iord[1]='\0';
  typex[0] = 'g'; typex[1] = '\0';

  CheckRhs(1,3);

  Type=VarType(1);
  switch ( VarType(1)) 
    {
    case 10 : 
      GetRhsVar(1,"S",&m1,&n1,&S);
      break;
    case 1 :
      GetRhsVar(1,"d",&m1,&n1,&l1);
      break;
    case 8:
      GetRhsVar(1,"I",&m1,&n1,&Im);
      break;
    default :
      Scierror(999,"%s: first argument has a wrong type, expecting scalar or string matrix\r\n",fname);
      return 0;
    }

  if (Rhs >= 2) 
    {
      char c;
      GetRhsVar(2,"c",&m2,&n2,&l2);
      if ( m2 == 0 ) {
	Scierror(999,"%s: second argument is an empty string\r\n",fname);
	return 0;
      }
      c = *cstk(l2);
      if (c != 'r' && c != 'c' && c != 'g' && c != 'l') {
	Scierror(999,"%s: second argument has a wrong value %s should be in r,c,g,lr,lc\r\n",fname,cstk(l2));
	return 0;
      }
      strcpy(typex,cstk(l2));
    }

  if (Rhs >= 3) 
    {
      GetRhsVar(3,"c",&m3,&n3,&l3);
      CheckLength(3,m3,1);
      if ( *cstk(l3) != 'i' && *cstk(l3) != 'd') 
	{
	  Scierror(999,"%s: third argument must be \"i\" or \"d\"\r\n",fname);
	  return 0;
	}
      iord[0] = *cstk(l3);
    }


  /** Scalar matrix **/
  iflag = 0;
  if (Lhs == 2) { iflag = 1; }

  if ( typex[0] == 'l') 
    {
      if (typex[1] == 'r') {
	CreateVar(Rhs+1,"i",&m1,&un,&lex);
      } else  {
	CreateVar(Rhs+1,"i",&un,&n1,&lex);
      }
    }
  else
  {
    CreateVar(Rhs+1,"i",&m1,&n1,&lex);
  }

  LhsVar(1)=1;
  if (Type==1) 
  {
    C2F(gsortd)(stk(l1),istk(lex),&iflag,&m1,&n1,typex,iord);
  }
  else if (Type==8)
  {
    if (Im.it==4)
    {
      C2F(gsortint)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    }
    else if(Im.it == 14)
    {
      C2F(gsortuint)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    }
    else if (Im.it==2)
    {
      C2F(gsortshort)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    }
    else if(Im.it == 12)
    {
      C2F(gsortushort)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    }
    else if (Im.it==1)
    {
      C2F(gsortchar)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    }
    else if(Im.it == 11)
    {
      C2F(gsortuchar)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    }
  }
  else if (Type==10) {
    C2F(gsorts)(S,istk(lex),&iflag,&m1,&n1,typex,iord);
    CreateVarFromPtr(Rhs+Lhs,"S", &m1, &n1, S);
    LhsVar(1)=Rhs+Lhs;
  }

  if ( Lhs == 2 ) { LhsVar(2)=Rhs+1 ; }
  C2F(putlhsvar)();
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
