/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

static void CNAME(ColSort,double)();
static void CNAME(RowSort,double)();
static void CNAME(GlobalSort,double)();
static void CNAME(LexiRow,double)();
static void CNAME(LexiCol,double)();

/******************************************************
 * Generic code for Sorting Matrices a[i+n*j] 
 * This code is inserted in gsort.c 
 * with double == double or type = int 
 ******************************************************/

static int CNAME(swapcode,double)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n;
  register double *pi = (double *) (parmi); 		
  register double *pj = (double *) (parmj); 
  do { 						
    register double t = *pi;		
    *pi++ = *pj;				
    *pj++ = t;				
  } while (--i > 0);				
  return(0);
}

static int CNAME(compareC,double)(i,j)
     char *i,*j;
{
  if ( *((double *)i) > *((double *)j))
    return (1);
  if ( *((double *)i) < *((double *)j))
    return (-1);
  return (0);
}

static int CNAME(compareD,double)(i,j)
     char *i,*j;
{
  if ( *((double *)i) < *((double *)j))
    return (1);
  if ( *((double *)i) > *((double *)j))
    return (-1);
  return (0);
}

/******************************************************
 * Column sort of a matrix 
 ******************************************************/

static void CNAME(ColSort,double)(a,ind,flag,n,p,dir)
     double *a;
     int *ind;
     int flag,n,p;
     char dir;
{
  int i,j;
  if ( flag == 1) 
    {
      for ( j= 0 ; j < p ; j++ ) 
	{
	  for ( i = 0 ; i < n ; i++) 
	    ind[i+n*j]= i+1;
	}
    }
  for ( j= 0 ; j < p ; j++ ) 
    {
      sciqsort((char *) (a+n*j),(char *) (ind+n*j),flag, n, 
	       sizeof(double),sizeof(int), 
	       (dir == 'i' ) ? CNAME(compareC,double) : CNAME(compareD,double),
	       CNAME(swapcode,double),swapcodeind);
    }
}

/******************************************************
 * Row sort of a matrix 
 ******************************************************/

static void CNAME(RowSort,double)(a,ind,flag,n,p,dir)
     double *a;
     int *ind;
     int n,p,flag;
     char dir;
{  
  int i,j;
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n ; i++) 
	{
	  for ( j= 0 ; j < p ; j++ ) 
	    {
	      ind[i+n*j]= j+1;
	    }
	}
    }
  for ( i = 0 ; i < n ; i++) 
    {
      sciqsort((char *) (a+i),(char *) (ind+i),flag, p, 
	       n*sizeof(double),n*sizeof(int), 
	       (dir == 'i' ) ? CNAME(compareC,double):CNAME(compareD,double),
	       CNAME(swapcode,double),swapcodeind);
    }
}


/******************************************************
 * Global sort of a Matrix
 ******************************************************/

static void CNAME(GlobalSort,double)(a,ind,flag,n,p,dir)
     double *a;
     int *ind;
     int n,p,flag;
     char dir;
{  
  int i;
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n*p ; i++) 
	ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n*p, 
	   sizeof(double),sizeof(int), 
	   (dir == 'i' ) ? CNAME(compareC,double):CNAME(compareD,double),
	   CNAME(swapcode,double),swapcodeind);
}

/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied 
 *  to sort them 
 ******************************************************/

static int CNAME(lexicols,double) =1;
static int CNAME(lexirows,double) =1;

static void CNAME(setLexiSize,double)(n,p) 
     int p,n;
{
  CNAME(lexicols,double) = p;
  CNAME(lexirows,double) = n;
}

static  int CNAME(LexiRowcompareC,double)(i,j)
     int *i; int *j;
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,double) ; jc++) 
    {
      if (*i > *j)
	return (1);
      if (*i < *j)
	return (-1);
      i += CNAME(lexirows,double);
      j += CNAME(lexirows,double);
    }
  return (0);
}
static  int CNAME(LexiRowcompareD,double)(i,j)
     int *i; int *j;
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,double) ; jc++) 
    {
      if (*i < *j)
	return (1);
      if (*i > *j)
	return (-1);
      i += CNAME(lexirows,double);
      j += CNAME(lexirows,double);
    }
  return (0);
}

static int CNAME(LexiRowswapcode,double)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n,j;
  register double *pi = (double *) (parmi); 		
  register double *pj = (double *) (parmj); 
  /** if ( n!= 1) printf(" swapcode avec n != 1\n"); **/
  do { 
    for ( j = 0 ; j < CNAME(lexicols,double) ; j++) 
      {
	register double t = *(pi +CNAME(lexirows,double)*j);		
	*(pi + CNAME(lexirows,double)*j) = *(pj+CNAME(lexirows,double)*j);				
	*(pj + CNAME(lexirows,double)*j) = t;	
      }
    pi++;
    pj++;
  } while (--i > 0);				
  return(0);
}


static void CNAME(LexiRow,double)(a,ind,flag,n,p,dir)
     int *a,*ind;
     int n,p;
     char dir;
{
  int i;
  CNAME(setLexiSize,double)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n ; i++) 
	  ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n, 
	   sizeof(double),sizeof(int), 
	   (dir == 'i' ) ? CNAME(LexiRowcompareC,double):CNAME(LexiRowcompareD,double),
	   CNAME(LexiRowswapcode,double),swapcodeind);
}

/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied 
 *  to sort them 
 ******************************************************/

static  int CNAME(LexiColcompareC,double)(i,j)
     double *i,*j;
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,double) ; ic++) 
    {
      if (*i > *j)
	return (1);
      if (*i < *j)
	return (-1);
      i++;
      j++;
    }
  return (0);
}
static  int CNAME(LexiColcompareD,double)(i,j)
     double *i,*j;
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,double) ; ic++) 
    {
      if (*i < *j)
	return (1);
      if (*i > *j)
	return (-1);
      i++;
      j++;
    }
  return (0);
}

static int CNAME(LexiColswapcode,double)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n,ir;
  register double *pi = (double *) (parmi); 		
  register double *pj = (double *) (parmj); 
  /** if ( n!= 1) printf(" swapcode avec n != 1\n"); **/
  do { 
    for ( ir = 0 ; ir < CNAME(lexirows,double) ; ir++) 
      {
	register double t = *(pi +ir);		
	*(pi +ir) = *(pj+ir);				
	*(pj +ir) = t;	
      }
    pi += CNAME(lexirows,double) ;
    pj += CNAME(lexirows,double) ;
  } while (--i > 0);				
  return(0);
}


static void CNAME(LexiCol,double)(a,ind,flag,n,p,dir)
     double *a;
     int *ind;
     int n,p;
     char dir;
{
  int i;
  CNAME(setLexiSize,double)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < p ; i++) 
	  ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, p, 
	   n*sizeof(double),sizeof(int), 
	   (dir == 'i' ) ? CNAME(LexiColcompareC,double):CNAME(LexiColcompareD,double),
	   CNAME(LexiColswapcode,double),
	   swapcodeind);
}




static void CNAME(inita,double)();
static void CNAME(afficher,double)();
static void CNAME(sorttest,double)();

static double CNAME(aa,double)[4*4] = {4,4,1,1,6,7,2,1,3,4,5,2,9,8,7,6};
/*static double aa[4*4] = {6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5}; */

static void CNAME(inita,double)(a,n,p)
     double *a;
     int n,p;
{
  int i;
  if ( n == 4 && p == 4  ) 
    for (i=0; i < n*p; i++) a[i]=CNAME(aa,double)[i];
  else 
    for (i=0; i < n*p; i++) a[i]=n*p-i;
  CNAME(afficher,double)(a,"a",n,p,sizeof(double));
}


static void CNAME(afficher,double)(a,name,n,p)
     char *name;
     double *a;
     int n,p;
{
  int i,j;
  printf("%s=\n",name); 
  for ( i = 0 ; i < n ; i++) 
    {
      for ( j= 0 ; j < p ; j++ ) 
	{
	  printf("%4.2f ", a[i+n*j]);
	}
      printf("\n");
    }
}

#define Ndoub 2
#define Pdoub 2 

static void CNAME(sorttest,double)()
{
  double a[Ndoub*Pdoub];
  int ind[Ndoub*Pdoub];
  int flag;
  int n=Ndoub,p=Pdoub;
  flag=1;

  /** Global sort example **/
  CNAME(inita,double)(a,n,p) ;
  CNAME(GlobalSort,double)(a,ind,flag,n,p,'c');
  CNAME(afficher,double)(a,"glob a",n,p);
  afficherint(ind,"glob ind",n,p);

  /** Column sort example **/
  CNAME(inita,double)(a,n,p) ;
  CNAME(ColSort,double)(a,ind,flag,n,p,'c');
  CNAME(afficher,double)(a,"col a",n,p);
  afficherint(ind,"col ind",n,p);

  /** Row sort example **/
  CNAME(inita,double)(a,n,p) ;
  CNAME(RowSort,double)(a,ind,flag,n,p,'c');
  CNAME(afficher,double)(a,"row a",n,p);
  afficherint(ind,"row ind",n,p);

  /** Lexicographic Col sort **/
  CNAME(inita,double)(a,n,p) ;
  CNAME(LexiCol,double)(a,ind,flag,n,p,'c');
  CNAME(afficher,double)(a,"lexico col a",n,p);
  afficherint(ind,"lexico col ind",1,p);

  /** Lexicographic Row sort **/
  CNAME(inita,double)(a,n,p) ;
  CNAME(LexiRow,double)(a,ind,flag,n,p,'c');
  CNAME(afficher,double)(a,"lexico Row a",n,p);
  afficherint(ind,"lexico Row ind",n,1);
}







