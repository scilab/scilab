/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

static void CNAME(ColSort,int)();
static void CNAME(RowSort,int)();
static void CNAME(GlobalSort,int)();
static void CNAME(LexiRow,int)();
static void CNAME(LexiCol,int)();


/******************************************************
 * Generic code for Sorting Matrices a[i+n*j] 
 * This code is inserted in gsort.c 
 * with int == double or type = int 
 ******************************************************/

static int CNAME(swapcode,int)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n;
  register int *pi = (int *) (parmi); 		
  register int *pj = (int *) (parmj); 
  do { 						
    register int t = *pi;		
    *pi++ = *pj;				
    *pj++ = t;				
  } while (--i > 0);				
  return(0);
}

static int CNAME(compareC,int)(i,j)
     char *i,*j;
{
  if ( *((int *)i) > *((int *)j))
    return (1);
  if ( *((int *)i) < *((int *)j))
    return (-1);
  return (0);
}

static int CNAME(compareD,int)(i,j)
     char *i,*j;
{
  if ( *((int *)i) < *((int *)j))
    return (1);
  if ( *((int *)i) > *((int *)j))
    return (-1);
  return (0);
}

/******************************************************
 * Column sort of a matrix 
 ******************************************************/

static void CNAME(ColSort,int)(a,ind,flag,n,p,dir)
     int *a;
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
	       sizeof(int),sizeof(int), 
	       (dir == 'i' ) ? CNAME(compareC,int) : CNAME(compareD,int),
	       CNAME(swapcode,int),swapcodeind);
    }
}

/******************************************************
 * Row sort of a matrix 
 ******************************************************/

static void CNAME(RowSort,int)(a,ind,flag,n,p,dir)
     int *a;
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
	       n*sizeof(int),n*sizeof(int), 
	       (dir == 'i' ) ? CNAME(compareC,int):CNAME(compareD,int),
	       CNAME(swapcode,int),swapcodeind);
    }
}


/******************************************************
 * Global sort of a Matrix
 ******************************************************/

static void CNAME(GlobalSort,int)(a,ind,flag,n,p,dir)
     int *a;
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
	   sizeof(int),sizeof(int), 
	   (dir == 'i' ) ? CNAME(compareC,int):CNAME(compareD,int),
	   CNAME(swapcode,int),swapcodeind);
}

/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied 
 *  to sort them 
 ******************************************************/

static int CNAME(lexicols,int) =1;
static int CNAME(lexirows,int) =1;

static void CNAME(setLexiSize,int)(n,p) 
     int p,n;
{
  CNAME(lexicols,int) = p;
  CNAME(lexirows,int) = n;
}

static  int CNAME(LexiRowcompareC,int)(i,j)
     int *i;
     int *j;
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,int) ; jc++) 
    {
      if (*i > *j)
	return (1);
      if (*i < *j)
	return (-1);
      i += CNAME(lexirows,int);
      j += CNAME(lexirows,int);
    }
  return (0);
}
static  int CNAME(LexiRowcompareD,int)(i,j)
     int *i;
     int *j;
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,int) ; jc++) 
    {
      if (*i < *j)
	return (1);
      if (*i > *j)
	return (-1);
      i += CNAME(lexirows,int);
      j += CNAME(lexirows,int);
    }
  return (0);
}

static int CNAME(LexiRowswapcode,int)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n,j;
  register int *pi = (int *) (parmi); 		
  register int *pj = (int *) (parmj); 
  if ( n!= 1) printf(" swapcode avec n != 1\n");
  do { 
    for ( j = 0 ; j < CNAME(lexicols,int) ; j++) 
      {
	register int t = *(pi +CNAME(lexirows,int)*j);		
	*(pi + CNAME(lexirows,int)*j) = *(pj+CNAME(lexirows,int)*j);				
	*(pj + CNAME(lexirows,int)*j) = t;	
      }
    pi++;
    pj++;
  } while (--i > 0);				
  return(0);
}


static void CNAME(LexiRow,int)(a,ind,flag,n,p,dir)
     int *a,*ind;
     int n,p;
     char dir;
{
  int i;
  CNAME(setLexiSize,int)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n ; i++) 
	  ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n, 
	   sizeof(int),sizeof(int), 
	   (dir == 'i' ) ? CNAME(LexiRowcompareC,int):CNAME(LexiRowcompareD,int),
	   CNAME(LexiRowswapcode,int),swapcodeind);
}

/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied 
 *  to sort them 
 ******************************************************/

static  int CNAME(LexiColcompareC,int)(i,j)
     int *i,*j;
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,int) ; ic++) 
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
static  int CNAME(LexiColcompareD,int)(i,j)
     int *i,*j;
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,int) ; ic++) 
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

static int CNAME(LexiColswapcode,int)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n,ir;
  register int *pi = (int *) (parmi); 		
  register int *pj = (int *) (parmj); 
  /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
  do { 
    for ( ir = 0 ; ir < CNAME(lexirows,int) ; ir++) 
      {
	register int t = *(pi +ir);		
	*(pi +ir) = *(pj+ir);				
	*(pj +ir) = t;	
      }
    pi += CNAME(lexirows,int) ;
    pj += CNAME(lexirows,int) ;
  } while (--i > 0);				
  return(0);
}


static void CNAME(LexiCol,int)(a,ind,flag,n,p,dir)
     int *a;
     int *ind;
     int n,p;
     char dir;
{
  int i;
  CNAME(setLexiSize,int)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < p ; i++) 
	  ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, p, 
	   n*sizeof(int),sizeof(int), 
	   (dir == 'i' ) ? CNAME(LexiColcompareC,int):CNAME(LexiColcompareD,int),
	   CNAME(LexiColswapcode,int),
	   swapcodeind);
}



static void CNAME(afficher,int)();
static void CNAME(sorttest,int)();
static void CNAME(inita,int)();

static int CNAME(aa,int)[4*4] = {4,4,1,1,6,7,2,1,3,4,5,2,9,8,7,6};
/*static int aa[4*4] = {6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5}; */

static void CNAME(inita,int)(a,n,p)
     int *a;
     int n,p;
{
  int i;
  if ( n == 4 && p == 4  ) 
    for (i=0; i < n*p; i++) a[i]=CNAME(aa,int)[i];
  else 
    for (i=0; i < n*p; i++) a[i]=n*p-i;
  CNAME(afficher,int)(a,"a",n,p,sizeof(int));
}


static void CNAME(afficher,int)(a,name,n,p)
     char *name;
     int *a;
     int n,p;
{
  int i,j;
  printf("%s=\n",name);
  for ( i = 0 ; i < n ; i++) 
    {
      for ( j= 0 ; j < p ; j++ ) 
	{
	  printf("%4.2d ", a[i+n*j]);
	}
      printf("\n");
    }
}


#define Nint 2
#define Pint 2 


static void CNAME(sorttest,int)()
{
  int a[Nint*Pint];
  int ind[Nint*Pint];
  int flag;
  int n=Nint,p=Pint;
  flag=1;

  /** Global sort example **/
  CNAME(inita,int)(a,n,p) ;
  CNAME(GlobalSort,int)(a,ind,flag,n,p,'c');
  CNAME(afficher,int)(a,"glob a",n,p);
  afficherint(ind,"glob ind",n,p);

  /** Column sort example **/
  CNAME(inita,int)(a,n,p) ;
  CNAME(ColSort,int)(a,ind,flag,n,p,'c');
  CNAME(afficher,int)(a,"col a",n,p);
  afficherint(ind,"col ind",n,p);

  /** Row sort example **/
  CNAME(inita,int)(a,n,p) ;
  CNAME(RowSort,int)(a,ind,flag,n,p,'c');
  CNAME(afficher,int)(a,"row a",n,p);
  afficherint(ind,"row ind",n,p);

  /** Lexicographic Col sort **/
  CNAME(inita,int)(a,n,p) ;
  CNAME(LexiCol,int)(a,ind,flag,n,p,'c');
  CNAME(afficher,int)(a,"lexico col a",n,p);
  afficherint(ind,"lexico col ind",1,p);

  /** Lexicographic Row sort **/
  CNAME(inita,int)(a,n,p) ;
  CNAME(LexiRow,int)(a,ind,flag,n,p,'c');
  CNAME(afficher,int)(a,"lexico Row a",n,p);
  afficherint(ind,"lexico Row ind",n,1);
}







