/*-----------------------------------------------------------------------------------*/
/* INRIA Scilab */
/*-----------------------------------------------------------------------------------*/
static int CNAME(swapcode,int)(char *parmi,char* parmj,int n, int inc )
{ 		
  int i = n;
  register int *pi = (int *) (parmi); 		
  register int *pj = (int *) (parmj); 
  do { 						
    register int t = *pi;		
    *pi++ = *pj;				
    *pj++ = t;	
    pi += inc;
    pj += inc;			
  } while (--i > 0);				
  return(0);
}
/*-----------------------------------------------------------------------------------*/
static int CNAME(compareC,int)(char *i,char *j)
{
  if ( *((int *)i) > *((int *)j))
    return (1);
  if ( *((int *)i) < *((int *)j))
    return (-1);
  return (0);
}
/*-----------------------------------------------------------------------------------*/
static int CNAME(compareD,int)(char *i,char *j)
{
  if ( *((int *)i) < *((int *)j))
    return (1);
  if ( *((int *)i) > *((int *)j))
    return (-1);
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/******************************************************
 * Column sort of a matrix 
 ******************************************************/
void CNAME(ColSort,int)( int * a, int *ind, int flag, int  n, int p,char dir)
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
/*-----------------------------------------------------------------------------------*/
/******************************************************
 * Row sort of a matrix 
 ******************************************************/
void CNAME(RowSort,int)( int * a, int *ind, int flag, int  n, int p,char dir)
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
/*-----------------------------------------------------------------------------------*/
/******************************************************
 * Global sort of a Matrix
 ******************************************************/
void CNAME(GlobalSort,int)( int * a, int *ind, int flag, int  n, int p,char dir)
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
/*-----------------------------------------------------------------------------------*/
/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied 
 *  to sort them 
 ******************************************************/

static int CNAME(lexicols,int) =1;
static int CNAME(lexirows,int) =1;

static void CNAME(setLexiSize,int)(int n,int p) 
{
  CNAME(lexicols,int) = p;
  CNAME(lexirows,int) = n;
}
/*-----------------------------------------------------------------------------------*/
static  int CNAME(LexiRowcompareC,int)(int *i,int *j)
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
static  int CNAME(LexiRowcompareD,int)(int *i, int*j)
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
/*-----------------------------------------------------------------------------------*/
static int CNAME(LexiRowswapcode,int)(char *parmi,char * parmj,int n) 
{ 		
  int i = n,j;
  register int *pi = (int *) (parmi); 		
  register int *pj = (int *) (parmj); 
  /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
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
/*-----------------------------------------------------------------------------------*/
void CNAME(LexiRow,int)( int * a, int *ind, int flag, int  n, int p,char dir)
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
/*-----------------------------------------------------------------------------------*/
/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied 
 *  to sort them 
 ******************************************************/
static  int CNAME(LexiColcompareC,int)(int *i,int *j)
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
/*-----------------------------------------------------------------------------------*/
static  int CNAME(LexiColcompareD,int)(int *i,int *j)
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
/*-----------------------------------------------------------------------------------*/
static int CNAME(LexiColswapcode,int)(char *parmi,char* parmj,int n) 
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
/*-----------------------------------------------------------------------------------*/
void CNAME(LexiCol,int)( int * a, int *ind, int flag, int  n, int p,char dir)
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
/*-----------------------------------------------------------------------------------*/
