/**************************************************** 
 * interface for string matrix primitives 
 * Copyright Inria/Enpc 
 * 
 * Note: in this interface arguments are transmited by 
 *       reference. interfaces must take care not to 
 *       change transmited arguments. 
 * Rewriten from scratch from strelm.f except for 
 *    intmacrostring
 *    intlibstring
 * Jean-Philippe Chancelier 2002 
 ******************************************************/ 

#include <ctype.h> 
#include <string.h>
#include <stdio.h>
#include "../stack-c.h"

static integer cx1 = 1;

/**************************************************** 
 * [r,w]=grep(S1,S2) 
 ******************************************************/ 

static int intgrep(char* fname)
{
  int m1,n1,m2,n2,un=1,l3,l4,l5,l6,mn,i,j,count;
  char **Str1,**Str2;
  CheckRhs(2,2);
  CheckLhs(1,2);

  GetRhsVar(1,"S",&m1,&n1,&Str1);
  GetRhsVar(2,"S",&m2,&n2,&Str2);
  mn = m1*n1; 
  CreateVar(3,"d",&mn,&un,&l3); 
  if  (Lhs == 2)  CreateVar(4,"d",&mn,&un,&l4); 
  count = 0; 
  for ( i = 0 ; i < mn ; i++ ) 
    {
      for ( j = 0 ; j < m2*n2 ; j++ ) 
	if ( strstr( Str1[i],Str2[j]) != NULL) 
	  {
	    *stk(l3+count) = i+1; 
	    if ( Lhs == 2 ) *stk(l4+count) = j+1;
	    count++; 
	    break;
	  }
    }
  if ( Lhs == 1 ) 
    {
      CreateVarFrom(4,"d",&un,&count,&l4,&l3); 
      LhsVar(1)=4; 
    }
  else 
    {
      CreateVarFrom(5,"d",&un,&count,&l5,&l3); 
      CreateVarFrom(6,"d",&un,&count,&l6,&l4); 
      LhsVar(1)=5; 
      LhsVar(2)=6; 
    }
  FreeRhsSVar(Str1);
  FreeRhsSVar(Str2);
  return 0;
}

/**************************************************** 
 * y= ascii(x) 
 * x : intXXX or constant matrix or srinng matrix 
 * 
 ******************************************************/

static int intascii(char* fname) 
{ 
  static int un=1,inc=1;
  int m1,n1,l1,mn,ichar=I_UCHAR,l2,i,nchars=0,k=0,j;
  char **Str;
  SciIntMat M;
  CheckRhs(1,1);
  CheckLhs(1,1);

  switch ( VarType(1)) {
  case 1 :
    GetRhsVar(1,"d",&m1,&n1,&l1);
    mn = m1*n1;  
    /* we create a string of the same length */
    CreateVar(2,"c",&mn,&un,&l2);
    for (i=0 ; i < mn ; i++ ) 
      *cstk(l2+i) = (int) *stk(l1+i);
    LhsVar(1) = 2  ;
    break;
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
    for ( i = 0 ; i < mn ; i++ ) 
      nchars += strlen(Str[i]);
    CreateVar(2,"d",&un,&nchars,&l2);
    k=0;
    for ( i = 0 ; i < mn ; i++ ) 
      for ( j =0 ; j < strlen(Str[i]) ; j++ ) 
	*stk(l2+k++) = (unsigned char) Str[i][j];
    FreeRhsSVar(Str);
    LhsVar(1) = 2  ;
    break; 
  case 8 :
    GetRhsVar(1,"I",&m1,&n1,&M);
    mn = m1*n1;  
    /* we create a string of the same length */
    CreateVar(2,"c",&mn,&un,&l1);
    /* from intxx to char */
    C2F(tpconv)(&M.it,&ichar,&mn, M.D, &inc, cstk(l1), &inc);
    LhsVar(1) = 2  ;
    break;
  default : 
    OverLoad(1);
    return 0;
  }
  return(0);
}

/**************************************************** 
 * y= strcat(str1,sep,[op]) 
 * op = 'c' or 'r'
 ******************************************************/

static int intstrcat(char* fname) 
{ 
  char typ = '*';
  char **Str,**Str1;
  static char def_sep[] ="";
  char *sep = def_sep;
  static int un=1;
  int m1,n1,mn,i,j,k,m2,n2,l2,m3,n3,l3,nchars=0;

  CheckRhs(1,3);
  CheckLhs(1,1);

  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
    if (Rhs >= 2) {
      GetRhsVar(2,"c",&m2,&n2,&l2);
      sep = cstk(l2);
    }
    if (Rhs >= 3) {
      GetRhsVar(3,"c",&m3,&n3,&l3);
      if ( m3*n3 != 0) 
	typ = cstk(l3)[0];
      if (typ != 'c' && typ != 'r' ) {
	Scierror(999,"%s: third argument should be 'c' or 'r'\r\n",fname);
	return 0;
      }
    }
    switch ( typ ) {
    case '*' : 
      /* just return one string */ 
      for ( i = 0 ; i < mn ; i++ ) 
	nchars += strlen(Str[i]);
      nchars += (mn-1)*strlen(sep);
      CreateVar(Rhs+1,"c",&un,&nchars,&l3);
      k=0;
      for ( i = 0 ; i < mn ; i++ ) 
	{
	  for ( j =0 ; j < strlen(Str[i]) ; j++ ) 
	    *cstk(l3+ k++) = Str[i][j];
	  if ( i != mn-1) 
	    for ( j =0 ; j < strlen(sep) ; j++ ) 
	      *cstk(l3+ k++) = sep[j];
	}
      FreeRhsSVar(Str);
      LhsVar(1) = Rhs+1  ;
      break;
    case 'c': 
      /* return a column matrix */ 
      if ( (Str1 = malloc((m1+1)*sizeof(char *)))==NULL) 
	{
	  Scierror(999,"%s: Out of memory\r\n",fname);
	  return 0;
	}
      Str1[m1]=NULL;
      for (i= 0 ; i < m1 ; i++) {
	/* length of row i */ 
	nchars = 0;
	for ( j = 0 ; j < n1 ; j++ ) 
	  nchars += strlen(Str[i+ m1*j]);
	nchars += (n1-1)*strlen(sep);
	if ( (Str1[i]=malloc((nchars+1)*sizeof(char)))==NULL) 
	  {
	    Scierror(999,"%s: Out of memory\r\n",fname);
	    return 0;
	  }
	/* fill the string */ 
	strcpy(Str1[i],Str[i]); 
	for ( j = 1 ; j < n1 ; j++ ) {
	  strcat(Str1[i],sep);
	  strcat(Str1[i],Str[i+ m1*j]);
	}
      }
      CreateVarFromPtr(Rhs+1,"S", &m1, &un, Str1);
      FreeRhsSVar(Str);
      LhsVar(1) = Rhs+1  ;
      FreeRhsSVar(Str1);
      break;
    case 'r': 
      /* return a row matrix */ 
      if ( (Str1 = malloc((n1+1)*sizeof(char *)))==NULL) 
	{
	  Scierror(999,"%s: Out of memory\r\n",fname);
	  return 0;
	}
      Str1[n1]=NULL;
      for (j= 0 ; j < n1 ; j++) {
	/* length of col j */ 
	nchars = 0;
	for ( i = 0 ; i < m1 ; i++ ) 
	  nchars += strlen(Str[i+ m1*j]);
	nchars += (m1-1)*strlen(sep);
	if ( (Str1[j]=malloc((nchars+1)*sizeof(char)))==NULL) 
	  {
	    Scierror(999,"%s: Out of memory\r\n",fname);
	    return 0;
	  }
	/* fill the string */ 
	strcpy(Str1[j],Str[j*m1]); 
	for ( i = 1 ; i < m1 ; i++ ) {
	  strcat(Str1[j],sep);
	  strcat(Str1[j],Str[i+ m1*j]);
	}
      }
      CreateVarFromPtr(Rhs+1,"S", &un, &n1, Str1);
      FreeRhsSVar(Str);
      FreeRhsSVar(Str1);
      LhsVar(1) = Rhs+1  ;
      break;
    }
    break; 
  default : 
    OverLoad(1);
    break;
  }
  return 0;
}

/**************************************************** 
 * y= str2code(str1) 
 ******************************************************/

static int intstr2code(char* fname) 
{ 
  static int un=1;
  int m1,n1,l1,mn,l2;

  CheckRhs(1,1);
  CheckLhs(1,1);

  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"c",&m1,&n1,&l1);
    mn = m1*n1;
    /* sciprint("string %s\r\n",cstk(l1)); */
    CreateVar(2,"i",&mn,&un,&l2);
    C2F(asciitocode)(&mn,istk(l2),cstk(l1),&un,mn); 
    /* take care of ref argument 1 */
    LhsVar(1) = 2; 
    break; 
  default : 
    OverLoad(1);
    break;
  }
  return 0;
}

/**************************************************** 
 * y= code2str(str1) 
 ******************************************************/

static int intcode2str(char* fname) 
{ 
  static int un=1;
  int m1,n1,l1,mn,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);

  switch ( VarType(1)) {
  case 1 :
    GetRhsVar(1,"i",&m1,&n1,&l1);
    mn = m1*n1;
    CreateVar(2,"c",&mn,&un,&l2);
    C2F(codetoascii)(&mn,istk(l1),cstk(l2),mn); 
    /* take care of ref argument 1 */
    LhsVar(1) = 2; 
    break; 
  default : 
    OverLoad(1);
    break;
  }
  return 0;
}


/**************************************************** 
 * y= intstrindex(str1,strmat) 
 ******************************************************/

static int intstrindex(char* fname) 
{
  static int un=1,zero=0;
  char *loc ; 
  char **Str;
  int m1,n1,l1,m2,n2,l3,i,count=0;

  CheckRhs(2,2);
  CheckLhs(1,1);

  GetRhsVar(1,"c",&m1,&n1,&l1); 
  if ( m1 == 0 ) 
    {
      CreateVar(3,"d",&zero,&zero,&l3);
      LhsVar(1) = 3; 
      return 0;
    }
  GetRhsVar(2,"S",&m2,&n2,&Str);
  CreateVar(3,"d",&un,&m1,&l3);
  /* working area  */ 
  for ( i = 0 ; i < m2*n2 ; i++) 
    {
      char *needle = Str[i]; 
      loc = cstk(l1);
      while (loc != NULL) 
	{
	  loc = strstr(loc,needle); 
	  if ( loc != NULL) 
	    {
	      *stk(l3+count++)=loc-cstk(l1)+1;
	      loc += 1; 
	    }
	}
    }
  /* resizing with computed size */ 
  CreateVar(3,"d",&un,&count,&l3);
  FreeRhsSVar(Str);
  /* WARNING:: must take care of arg 1 which is transmited by ref 
   * and was changed by the GetRhsVar 
   */
  LhsVar(1) = 3; 
  return 0;
}

/**************************************************** 
 * y= intstrsubst(matstr,search,replace) 
 * exec loader.sce ;
 * mysubs('pipo','pi','foo')
 ******************************************************/

static char ** subst(char **Str,int n,char *needle,char *replace) 
{
  int i,count,size,j;
  char **Str1;
  Str1 = malloc( (n+1)*sizeof(char*)); 
  if (Str1 == NULL) 
    {
      return NULL;
    }
  Str1[n]= NULL;
  for ( i = 0 ; i < n ; i++) 
    {
      char *loc = Str[i]; 
      int locsize= strlen(loc);
      count=0;
      while (loc != NULL) 
	{
	  loc = strstr(loc, needle); 
	  if ( loc != NULL) 
	    {
	      *loc = '\0'; /* we put a mark */
	      loc += 1; 
	      count++;
	    }
	}
      /* now we know the number of occurences = count */ 
      size = locsize + count*(strlen(replace)-strlen(needle))+1;
      Str1[i]= malloc( size*sizeof(char));
      if (Str1 == NULL) 
	{
	  /** XXXX */ 
	  return NULL;
	} 
      /* fill result */ 
      Str1[i][0]='\0';
      loc = Str[i];
      for ( j = 0 ; j < count ; j++ ) 
	{
	  strcat(Str1[i],loc);
	  loc += strlen(loc)+ strlen(needle);
	  strcat(Str1[i],replace);
	}
      strcat(Str1[i],loc);
    }
  return Str1;
}

static int intstrsubst(char* fname) 
{
  char *needle,*replace ; 
  char **Str,**Str1;
  int m1,n1,m2,n2,l2,m3,n3,l3;

  CheckRhs(3,3);
  CheckLhs(1,1);

  GetRhsVar(1,"S",&m1,&n1,&Str);
  GetRhsVar(2,"c",&m2,&n2,&l2);
  needle= cstk(l2);
  GetRhsVar(3,"c",&m3,&n3,&l3);
  replace = cstk(l3);

  Str1 = subst(Str,m1*n1,needle,replace); 
  if (Str1 == NULL) 
    {
      Scierror(999,"running out of memory\r\n");
      FreeRhsSVar(Str);
      return 0;
    }
  CreateVarFromPtr(4,"S",&m1,&n1,Str1);
  FreeRhsSVar(Str);
  FreeRhsSVar(Str1);
  /* take care of ref argument 1 */
  LhsVar(1) = 4; 
  return 0;
}

/**************************************************** 
 * y= length(x) 
 ******************************************************/

static int intlength(char* fname) 
{
  char **Str;
  static int un=1;
  int *header,m1,n1,l1,l2,mn,i;
  CheckRhs(1,1);
  CheckLhs(1,1);
  switch ( VarType(1)) {
  case 1 :
  case 2 :
  case 4 :
    header = GetData(1);
    CreateVar(2,"d",&un,&un,&l2);
    *stk(l2)=header[1]*header[2];
    LhsVar(1) = 2  ;
    break;
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
    CreateVar(2,"d",&m1,&n1,&l2);
    for ( i = 0 ; i < mn ; i++ ) 
      {
	*stk(l2+i)= strlen(Str[i]);
      }
    FreeRhsSVar(Str);
    LhsVar(1) = 2  ;
    break; 
  case 15 :
    GetRhsVar(1,"l",&m1,&n1,&l1);
    CreateVar(2,"d",&un,&un,&l2);
    *stk(l2)=m1;
    LhsVar(1) = 2  ;
    break;
  case 16:
    GetRhsVar(1,"t",&m1,&n1,&l1);
    CreateVar(2,"d",&un,&un,&l2);
    *stk(l2)=m1;
    LhsVar(1) = 2  ;
    break;
  case 17 :
    GetRhsVar(1,"m",&m1,&n1,&l1);
    CreateVar(2,"d",&un,&un,&l2);
    *stk(l2)=m1;
    LhsVar(1) = 2  ;
    break;
  default : 
    OverLoad(1);
    return 0;
  }
  return(0);
}


/**************************************************** 
 * y= convstr(x [,flag]) 
 ******************************************************/

static int intconvstr(char* fname) 
{
  char **Str;
  char flag = 'l'; 
  int m1,n1,m2,n2,l2,mn,i,j;
  CheckRhs(1,2);
  CheckLhs(1,1);

  GetRhsVar(1,"S",&m1,&n1,&Str);
  mn = m1*n1;  
  if (Rhs == 2) 
    {
      GetRhsVar(2,"c",&m2,&n2,&l2);
      flag = *cstk(l2) ; 
      if ( flag != 'l' && flag != 'u') 
	{
	  Scierror(999,"%s: second argument \"%s\", should be 'u' or 'l'\r\n",
		   fname,cstk(l2));
	  return 0;
	}
    }
  if ( flag == 'u' ) 
    for ( i = 0 ; i < mn ; i++ ) 
      for ( j=0 ; j < strlen(Str[i]) ; j++) 
	  Str[i][j]=toupper(Str[i][j]);
  else 
    for ( i = 0 ; i < mn ; i++ ) 
	for ( j=0 ; j < strlen(Str[i]) ; j++) 
	  Str[i][j]=tolower(Str[i][j]);
  CreateVarFromPtr(Rhs+1,"S",&m1,&n1,Str);
  FreeRhsSVar(Str);
  LhsVar(1) = Rhs+1; 
  return(0);
}



/**************************************************** 
 * y= part(matstr,v) 
 ******************************************************/

static char **part(char **Str,int n,int *Ind,int nI)
{
  int i,j,k;
  char **Str1;
  Str1 = malloc((n+1)*sizeof(char*)); 
  if (Str1 == NULL) 
    {
      return NULL;
    }
  Str1[n]= NULL;
  for ( i = 0 ; i < n ; i++) 
    {
      char *loc = Str[i]; 
      int locsize= strlen(loc);
      Str1[i]= malloc( (nI+1)*sizeof(char));
      if (Str1 == NULL) 
	{
	  return NULL;
	} 
      /* fill result */ 
      k = 0; 
      for ( j = 0 ; j < nI ; j++) 
	{
	  if ( Ind[j] > 0 && Ind[j] <= locsize ) 
	    Str1[i][k++]= Str[i][Ind[j]-1];
	  else 
	    Str1[i][k++]= ' ';
	}
      Str1[i][k]='\0';
    }
  return Str1;
}

static int intpart (char* fname) 
{
  char **Str,**Str1;
  int m1,n1,m2,n2,l2;
  
  CheckRhs(2,2)
  CheckLhs(1,1);

  GetRhsVar(1,"S",&m1,&n1,&Str);
  GetRhsVar(2,"i",&m2,&n2,&l2);

  Str1 = part(Str,m1*n1,istk(l2),m2*n2);
  if (Str1 == NULL) 
    {
      Scierror(999,"running out of memory\r\n");
      FreeRhsSVar(Str);
      return 0;
    }
  CreateVarFromPtr(3,"S",&m1,&n1,Str1);
  FreeRhsSVar(Str);
  FreeRhsSVar(Str1);
  LhsVar(1) = 3; 
  return 0;
}



/**************************************************** 
 * y= emptystr(....) 
 ******************************************************/

static char **empty(int n)
{
  int i;
  char **Str1;
  Str1 = malloc((n+1)*sizeof(char*)); 
  if (Str1 == NULL) 
    {
      return NULL;
    }
  Str1[n]= NULL;
  for ( i = 0 ; i < n ; i++) 
    {
      Str1[i]= malloc(sizeof(char));
      if (Str1 == NULL) 
	{
	  return NULL;
	} 
      Str1[i][0]='\0';
    }
  return Str1;
}

static int intemptystr(char* fname) 
{
  int *header;
  char **Str1;
  int m1,n1,l1,m2,n2,l2,m,n;
  CheckRhs(0,2)
  CheckLhs(1,1);

  switch ( Rhs ) {
  case 0 : 
    m=n=1; 
    break; 
  case 2: 
    GetRhsVar(1,"d",&m1,&n1,&l1);
    CheckScalar(1,m1,n1);
    GetRhsVar(2,"d",&m2,&n2,&l2);
    CheckScalar(1,m2,n2);
    m = *stk(l1);n = *stk(l2); 
    break;
  case 1:
    switch ( VarType(1)) {
    case 1 :
    case 2 :
    case 4 :
    case 10:
      header = GetData(1);
      m = header[1]; n = header[2];
      break;
    case 15 :
      GetRhsVar(1,"l",&m1,&n1,&l1);
      m = m1;n = 1; 
      break;
    case 16:
      GetRhsVar(1,"t",&m1,&n1,&l1);
      m = m1;n = 1; 
      break;
    case 17 :
      GetRhsVar(1,"m",&m1,&n1,&l1);
      m = m1;n = 1; 
      break;
    default : 
      OverLoad(1);
      return 0;
    }
  }
  Str1 = empty(m*n);
  if (Str1 == NULL) 
    {
      Scierror(999,"running out of memory\r\n");
      return 0;
    }
  CreateVarFromPtr(Rhs+1,"S",&m,&n,Str1);
  FreeRhsSVar(Str1);
  LhsVar(1) = Rhs+1; 
  return 0;
}

/*-----------------------------------------------------------
 * XXXXXX To be done 
 *-----------------------------------------------------------*/ 

static integer C2F(strord)(r1, l1, r2, l2)
     integer *r1, *l1, *r2, *l2;
{
    /* Initialized data */

    static integer blank = 40;

    
    integer ret_val, ix1;

    
    integer iord, ix, c1, c2, ll;

    /* Parameter adjustments */
    --r2;
    --r1;

    /* Function Body */

    iord = 0;
    if (*l1 == 0) {
	if (*l2 > 0) {
	    ret_val = -1;
	    return ret_val;
	} else {
	    ret_val = 0;
	    return ret_val;
	}
    } else {
	if (*l2 == 0) {
	    ret_val = 1;
	    return ret_val;
	}
    }
    ll = Min(*l1,*l2);
    ix1 = Max(*l1,*l2);
    for (ix = 1; ix <= ix1; ++ix) {
	if (ix <= *l1) {
	    c1 = r1[ix];
	} else {
	    c1 = blank;
	}
	if (ix <= *l2) {
	    c2 = r2[ix];
	} else {
	    c2 = blank;
	}
	if (c1 >= 0) {
	    c1 = 256 - c1;
	}
	if (c2 >= 0) {
	    c2 = 256 - c2;
	}
	if (c1 > c2) {
	    ret_val = 1;
	    return ret_val;
	} else if (c1 < c2) {
	    ret_val = -1;
	    return ret_val;
	}
/* L10: */
    }
    ret_val = 0;
    return ret_val;
} /* strord_ */



static int intsort(char* fname) 
{
  integer ix1;
  integer lind;
  integer tops, lszi;
  integer ix, l, m, n, lindi;
  integer l1, l2;
  integer il, mn, ls, lw;
  integer id1, il1;
  integer idi, inc, sel, ilr, vol, lsz, id1r;

  Scierror(999,"sort to be done \r\n");
    /* Function Body */
  
    if (Rhs > 2) {
	Error(42);
	return 0;
    }
    if (Lhs > 2) {
	Error(41);
	return 0;
    }
    tops = Top;
    /*     select type of sort to perform */
    sel = 0;
    if (Rhs == 2) {
	C2F(getorient)(&Top, &sel);
	if (Err > 0) {
	    return 0;
	}
	--Top;
    }
    if (sel == 2) {
	Top = tops;
	C2F(com).fun = -1;
	ix1 = iadr(*lstk(Top + 1 - Rhs ));
	C2F(funnam)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], "sort", &ix1, 4L);
	return 0;
    }
    if (Rhs == 2) {
	--Rhs;
    }
    C2F(ref2val)();

    il1 = iadr(*lstk(Top ));
    ilr = il1;
    m = *istk(il1 +1);
    n = *istk(il1 + 1 +1);
    mn = m * n;
    id1 = il1 + 4;
    l1 = id1 + mn + 1;
    vol = *istk(id1 + mn ) - 1;

    id1r = id1;
    ls = iadr(*lstk(Top +1));
    lsz = ls + vol;
    lind = lsz + mn;
    lw = lind + mn;
    Err = sadr(lw) - *lstk(Bot );
    if (Err > 0) {
	Error(17);
	return 0;
    }
    C2F(icopy)(&vol, istk(l1 ), &cx1, istk(ls ), &cx1);
    ix1 = mn - 1;
    for (ix = 0; ix <= ix1; ++ix) {
	*istk(lsz + ix ) = *istk(id1 + ix +1) - *istk(id1 + ix );
	/* L91: */
    }

    if (sel == 0) {
      /*     sort(a) <=> sort(a,'*') */
      C2F(rcsort)(C2F(strord), istk(lsz ), istk(id1 ), istk(ls ), &mn, istk(lind ));
    } else if (sel == 1) {
      /*     sort(a,'r')  <=>  sort(a,1) */
      lszi = lsz;
      idi = id1;
      lindi = lind;
      ix1 = n - 1;
      for (ix = 0; ix <= ix1; ++ix) {
	C2F(rcsort)(C2F(strord), istk(lszi ), istk(idi ), istk(ls ), &m, istk(lindi ));
	lszi += m;
	idi += m;
	lindi += m;
	/* L95: */
      }
    }
    l2 = ilr + 4 + mn + 1;
    ix1 = mn - 1;
    for (ix = 0; ix <= ix1; ++ix) {
      C2F(icopy)(istk(lsz + ix ), istk(ls - 1 + *istk(id1 + ix )), &cx1, istk(l2 ), &cx1);
      l2 += *istk(lsz + ix );
      /* L93: */
    }
    *lstk(Top +1) = sadr(l2);
    *istk(id1 ) = 1;
    ix1 = mn - 1;
    for (ix = 0; ix <= ix1; ++ix) {
      *istk(id1 + ix +1) = *istk(id1 + ix ) + *istk(lsz + ix );
      /* L94: */
    }
    if (Lhs == 1) {
      goto L999;
    }
    ++Top;
    il = iadr(*lstk(Top ));
    ix1 = il + 4;
    l = sadr(ix1);
    inc = -1;
    if (sadr(lind) > l) {
      inc = 1;
    }
    C2F(int2db)(&mn, istk(lind ), &inc, stk(l ), &inc);
    *istk(il ) = 1;
    *istk(il +1) = m;
    *istk(il + 1 +1) = n;
    *istk(il + 2 +1) = 0;
    *lstk(Top +1) = l + mn;
    goto L999;
 L999:
    return 0;
} /* intssort_ */

/*-----------------------------------------------------------
 * intstring 
 *-----------------------------------------------------------*/ 

static int intlibstring(void);
static int intmacrostring(void);

static int intstring (char* fname) 
{
  int * data,*info,*header,nstr;
  int lr,lc,m,n,it,size;
  CheckRhs(1,1);

  switch ( VarType(1)) {
  case 1 :
    /* scalar matrix case */ 
    CheckLhs(1,1);
    GetRhsCVar(1,"d",&it,&m,&n,&lr,&lc);
    if ( m*n == 0) 
      {
	LhsVar(1)=1; 
	return 0;
      }
    size = m * n + 1 ; 
    size +=  m * n * ((C2F(iop).lct[6] << 1) + 4);
    CreateData(2, (4+size)*sizeof(int));
    header = GetData(2); 
    info = header+4;
    data = header+4 + m * n + 1;
    if (it == 0) 
      C2F(cvdm)(stk(lr), &m, &m, &n, &C2F(iop).lct[6], 
		&C2F(iop).lct[5],data,info);
    else 
      C2F(cvwm)(stk(lr), stk(lc), &m, &m, &n, 
		&C2F(iop).lct[6], &C2F(iop).lct[5],data,info); 
    /* the real size */ 
    nstr= info[m*n] -1; 
    CreateData(2, (4+m*n+1+ nstr)*sizeof(int));
    header[0]=10; 
    header[1]=m;
    header[2]=n;
    header[3]=0;
    LhsVar(1)=2;
    break;
  case 10:    
    /* string matrix case 
     */
    CheckLhs(1,1);
    if ( IsRef(1) ) 
      {
	/* since the argument must be returned 
	 * we must replace the ref by its value 
	 */ 
	Ref2val(1,2);
	/* just to register 2 in the registered variables */ 
	header = GetData(2);
	/* we return 2 */ 
	LhsVar(1)=2; 
      }
    else 
      {
	/* just to register 1 in the registered variables */ 
	header = GetData(1);
	/* we return 1 */ 
	LhsVar(1) = 1; 
      }
    break;
  case 11 :
  case 13 :
    CheckLhs(1,3);
    intmacrostring();
    header = GetData(2);
    header = GetData(3);
    header = GetData(4);
    LhsVar(1)=2; 
    LhsVar(2)=3; 
    LhsVar(3)=4; 
    break;
  case 14:
    CheckLhs(1,1);
    intlibstring();
    header = GetData(2);
    LhsVar(1)=2;  
    break;
  default : 
    OverLoad(1);
    return 0;

  }
  return 0; 
}


/*-----------------------------------------------------------
 * special cases of intstring 
 * macros 
 *-----------------------------------------------------------*/ 

static int C2F(listnames)(integer pos,integer *il); 

static int intmacrostring(void)
{
  static integer eol = 99;
  integer ix1;
  integer ltxt, l, n;
  integer il, nl, lw, nch, ilm, ilt, ilp;
  /*     argument on top */
  il = iadr(*lstk(Top ));
  if (*istk(il ) < 0) {
    il = iadr(*istk(il +1));
  }
  ilm = il;
  ++il;
  /*     first extract lout */
  C2F(listnames)(Top + 1, &il);
  /*     first extract lin */
  C2F(listnames)(Top + 2, &il);
  /*     now extract function body */
  lw = *lstk(Top + 3);
  if (*istk(ilm ) == 13) {
    ltxt = lw;
    ilt = iadr(ltxt);
    *istk(ilt ) = 1;
    *istk(ilt + 1 +1) = 0;
    *istk(ilt + 2 +1) = 0;
    *istk(ilt + 3 +1) = 1;
    ilt += 4;
    *lstk(Top + 4) = sadr( ilt + 1);
    return 0;
  }
  ltxt = lw;
  ilt = iadr(ltxt);
  *istk(ilt ) = 10;
  *istk(ilt + 1 +1) = 1;
  *istk(ilt + 2 +1) = 0;
  *istk(ilt + 3 +1) = 1;
  ilp = ilt + 4;
  /*     compute number of lines of the macro */
  nch = *istk(il );
  nl = 0;
  ++il;
  l = il - 1;

  while (1) {
    ++l;
    if (*istk(l) == eol) {
      if (*istk(l +1) == eol) break; 
      ++nl;
    }
  }

  *istk(ilt +1) = nl;
  if (nl == 0) {
    *istk(ilt ) = 1;
    *istk(ilt + 2) = 0;
    *lstk(Top + 4) = sadr( ilt + 4);
    return 0;
  }
  
  ilt = ilp + nl + 1;
  l = il;
  
  while (1) 
    {
      if (*istk(l) != eol) { ++l; continue;} 
      if (*istk(l+1) == eol) {
	*lstk(Top + 4) = sadr(ilt + 1);
	return 0;
      }
      n = l - il;
      ++ilp;
      *istk(ilp ) = *istk(ilp - 2 +1) + n;
      C2F(icopy)(&n, istk(il ), &cx1, istk(ilt ), &cx1);
      ilt += n;
      il = l + 1;
      l = il;
    }
  return 0;
} 
/*-----------------------------------------------------------
 * special cases of intstring 
 * library 
 *-----------------------------------------------------------*/ 

static int intlibstring(void)
{
  static integer nclas = 29;
  integer ix1;
  integer k, l, n;
  integer l1, n1, il, nn, lw;
  integer ilr;

  il = iadr(*lstk(Top ));
  ilr = il;
  if (*istk(il ) < 0) {
    il = iadr(*istk(il +1));
  }
  lw = iadr(*lstk(Top +1));
  
  n1 = *istk(il +1);
  l1 = il + 2;
  il = il + n1 + 2;
  n = *istk(il );
  il = il + nclas + 2;
  ilr = lw;
  ix1 = ilr + 6 + n1 + n * (nlgh+1);
  Err = sadr(ix1) - *lstk(Bot );
  if (Err > 0) {
    Error(17);
    return 0;
  }
  *istk(ilr ) = 10;
  *istk(ilr +1) = n + 1;
  *istk(ilr + 1 +1) = 1;
  *istk(ilr + 2 +1) = 0;
  *istk(ilr + 3 +1) = 1;
  l = ilr + 6 + n;
  C2F(icopy)(&n1, istk(l1 ), &cx1, istk(l ), &cx1);
  *istk(ilr + 4 +1) = n1 + 1;
  l += n1;
  ix1 = n;
  for (k = 1; k <= ix1; ++k) {
    C2F(namstr)(istk(il ), istk(l ), &nn, &cx1);
    *istk(ilr + 5 + k ) = *istk(ilr + 4 + k ) + nn;
    l += nn;
    il += nsiz;
    /* L49: */
  }
  ix1 = ilr + l - ilr;
  *lstk(Top + 1 +1) = sadr(ix1);
  return 0;
} 


/*-------------------------------------------------------------
 *  utility 
 *  extract a set of names coded in istk(il+...) 
 *  and store the result in the stack as a
 *  string matrix store at position lstk(pos) 
 *  Note: the value of il is changed by this funtion 
 *-------------------------------------------------------------*/

static int C2F(listnames)(integer pos,integer *il)
{
  integer ix1;
  integer ilio;
  integer j, l, n, nn;
  integer ilp;

  n = *istk(*il );
  ++(*il);
  ilio = iadr(*lstk(pos ));
  *istk(ilio ) = 10;
  if (n == 0) {
    *istk(ilio ) = 1;
  }
  *istk(ilio +1) = Min(1,n);
  *istk(ilio + 1 +1) = n;
  *istk(ilio + 2 +1) = 0;
  ilp = ilio + 4;
  *istk(ilp ) = 1;
  l = ilp + n;
  if (n != 0) {
    ix1 = n;
    for (j = 1; j <= ix1; ++j) {
      C2F(namstr)(istk(*il ), istk(l +1), &nn, &cx1);
      l += nn;
      *istk(ilp + j ) = l + 1 - (ilp + n);
      *il += nsiz;
      /* L34: */
    }
  }
  ix1 = l + 1;
  *lstk(pos +1) = sadr(ix1);
  return 0;
} 







/*-----------------------------------------------------------
 * Interface for strings functions 
 *-----------------------------------------------------------*/ 

typedef int (*des_interf) __PARAMS((char *fname));

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} StrelmTable;
  

static StrelmTable Tab[]={
  {intlength,"length"},
  {intpart,"part"},
  {intstring,"string"},
  {intconvstr,"convstr"},
  {intemptystr,"emptystr"},
  {intstr2code,"str2code"},
  {intcode2str,"code2str"},
  {intsort,"sort"},
  {intstrcat,"strcat "},
  {intstrindex,"strindex"},
  {intstrsubst,"strsubst"},
  {intascii,"ascii"},
  {intgrep,"grep"},
};

int C2F(cstrelm)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name);
  C2F(putlhsvar)();
  return 0;
}

