/* Copyright INRIA */
/* basic management of a global variable list */

#include "gvar.h"
#include <stdlib.h>
#include <string.h>

/* CONSTANTS DEF */
#ifndef NULL
#define NULL 0
#endif

/* I want trace */
/* #define GVAR_TRACE */

/* size of a reallocation */
#define PAGESZ 20

#define gvar_nsiz (nsiz*4)
/* TYPES DEF */ /*******************************************/

/* the elements of the global var list */
typedef struct {
  Matrix *name;
  double *ptr;
} t_gvar;



/* GLOBAL STATIC VAR DEF */ /*******************************************/

/* the list itself */
static t_gvar *gvarLst=NULL;

/* the current size of the list */
static int gvarLstSz=0;
/* the smallest free element in the list */
static int curFreeElt=0;


/* SUBS DEF */ /*******************************************/



/********** reallocLst ***********/
int reallocLst()
     /* resize the gvar list */
     /* OUTPUT : result, if non zero, the operation has failed */
{
  t_gvar *newLst;
  
  newLst = (t_gvar *)realloc(gvarLst, (gvarLstSz +  PAGESZ) * sizeof(t_gvar));
  if (newLst == NULL ) return(-1);
  
  
  gvarLst = newLst;
  memset(gvarLst + gvarLstSz, (int)0, PAGESZ * sizeof(t_gvar) );
  gvarLstSz += PAGESZ;
  return(0);
}


/********** newRank ***********/
int newRank()
     /* get a free element in the gvar list*/
     /* OUTPUT : rank of the new element, negative if  */
     /*           the operation is impossible */
{
  int rank;
  int res;

  rank = curFreeElt;
      
  if ( curFreeElt<gvarLstSz )
    {
      do  /* find the next free element */
	curFreeElt++;
      while ( (curFreeElt < gvarLstSz) && ((gvarLst[curFreeElt]).ptr != NULL ) );
      return( rank );
    }
  
  res = reallocLst();
  if (res==0) 
    {
      curFreeElt++;
      return(rank);
    }
  else return(-1);
}

/********** updateElt **********/
#ifndef __STDC__
void updateElt(rank, Mname, objPtr)
     int rank;
     Matrix *Mname;
     double *objPtr;
#else /* __STDC__ */
     void updateElt( int rank, Matrix *Mname, double *objPtr )
#endif /* __STDC__ */
     /* update the fields of a gvar list element */
     /* INPUT  : -rank, element to update in the list */
     /*          -name, new name of the scilab variable */
     /*          -objPtr, new 'global' instance of the variable */
{
  
  /* memcpy( (gvarLst[rank]).name, name, gvar_nsiz * sizeof(int)); */
  (gvarLst[rank]).name = Mname;
  (gvarLst[rank]).ptr = objPtr;

}




/*********** newGvar **********/
#ifndef __STDC__
int newGvar(Mname, sciObj)
     Matrix *Mname;
     double *sciObj;
#else /* __STDC__ */
     int newGvar( Matrix *Mname, double *sciObj)
#endif /* __STDC__ */
     /* add a scilab object to the list */
     /* INPUT  : -name, name of the sci var */
     /*          -sciObj, pointer on the scilab var in the sci stack */
     /* OUTPUT : 0-> OK, 1-> no more stack, 2-> can not add new elt */
     /*           to the list */
{
  int objSz; /* size in bytes of the scilab object */
  int rank; /* rank of the object in the gvar list */
  double *objPtr; /* ptr on the actual global object */
  Matrix *objName;
  int objNameSz;

  /* get the memory for the object */
  objSz = MatrixMemSize( sciObj );
  objPtr = malloc( objSz * sizeof(char) );
  if ( objPtr == NULL ) return(1); /* no more heap memory free */
  objNameSz = MatrixMemSize( Mname );
  objName = (Matrix *) malloc( objNameSz * sizeof(char) );

  /* get a place in the global var list */
  rank = newRank();
  if (rank <0 ) return(2); /* unable to get a free element in the global var list */
  
  /* copy the object */
  memcpy(objPtr, sciObj, objSz);
  memcpy(objName, Mname, objNameSz);
  /* update the info in the list */
  updateElt( rank, objName, objPtr );
  return 0;
}


#ifndef __STDC__
int Mstrcmp(Ms1, Ms2)
     Matrix *Ms1;
     Matrix *Ms2;
#else /* __STDC__ */
     int Mstrcmp( Matrix *Ms1, Matrix *Ms2)
#endif /* __STDC__ */
{
  int sz1;
  int sz2;
  int *m1;
  int *m2;
 
  if ((Ms1==NULL) || (Ms2==NULL) ) return(-1);
  m1 = (int *)Ms1;
  m2 = (int *)Ms2;
 
  sz1 = m1[5] -1;
  sz2 = m2[5] -1;

  if (sz1 != sz2) return(-1);
  return(memcmp(m1+6, m2+6, sz1 * sizeof(int)));

}

/*********** findGvar **********/
#ifndef __STDC__
int findGvar(Mname)
     Matrix *Mname;
#else /* __STDC__ */
     int findGvar( Matrix *Mname )
#endif /* __STDC__ */
     /* find the rank of a global variable in the list */
     /* INPUT  : name, name of the scilab variable to find */
     /* OUTPUT : rank of the variable. -1 if not found */

{
  int rank=0;
  
  while ( (rank<gvarLstSz) && (Mstrcmp( Mname, ((gvarLst[rank]).name) ) != 0) )
    rank++;

  if (rank==gvarLstSz) return(-1);
  else return(rank);
  
}



/*********** setGvar **********/
#ifndef __STDC__
int setGvar(Mname, sciObj)
     Matrix *Mname;
     double *sciObj;
#else /* __STDC__ */
     int setGvar( Matrix *Mname, double *sciObj)
#endif /* __STDC__ */
     /* set a global variable */
     /* if it doesn't exist, its created */
     /* INPUT  : -name, scilab var name */
     /*          -sciObj, scilab object to store */
     /* OUTPUT : result of the operation. */
     /*          1  -> the variable replaced an old value */
     /*          0  -> the variable has been created and stored */
     /*          <0 -> the operation has failed */

	
{
  int objSz; /* size in bytes of the scilab object */
  int rank; /* rank of the object in the gvar list */
  double *objPtr; /* ptr on the actual global object */
  Matrix *objName;
  int objNameSz;
  
#ifdef GVAR_TRACE
  printf("In setGvar\n");
#endif
  rank = findGvar( Mname );
  
  if (rank>=0) 
    { /* object already exists */
      /* free the old object */
#ifdef GVAR_TRACE
      printf("   Object found at rank %d\n", rank);
#endif
      free( (gvarLst[rank]).ptr);
      free( (gvarLst[rank]).name);
      /* get the memory for the object */
      objSz = MatrixMemSize( sciObj );
      objPtr = (double *)malloc( objSz * sizeof(char) );
      if ( objPtr == NULL ) return(-1); /* no more heap memory free */
      objNameSz = MatrixMemSize( Mname );
      objName = (Matrix *) malloc( objNameSz * sizeof(char) );

      /* copy the object */
      memcpy(objPtr, sciObj, objSz);
      memcpy(objName, Mname, objNameSz);
  
      /* update the info in the list */
      updateElt( rank, objName, objPtr );
      return(1);
    }
  else
    { /* object must be created */
#ifdef GVAR_TRACE
      printf("   Object not found. Needs to be created\n");
#endif

      return( newGvar( Mname, sciObj) );
      
    }
}


/*********** getGvar **********/
#ifndef __STDC__
int getGvar(Mname, ptrObj)
     Matrix *Mname;
     double **ptrObj;
#else /* __STDC__ */
     int getGvar( Matrix *Mname, double **ptrObj)
#endif /* __STDC__ */
     /* get a global variable */
     /* INPUT  : -name, scilab var name */
     /*          -sciObj, scilab object to store */
     /* OUTPUT : result of the operation. */
     /*         -1  -> the variable didn't exist */
     /*              in this case, NULL is returned in ptrObj */
     /*          0  -> the variable has been read */
{
  int rank; /* rank of the object in the gvar list */
#ifdef GVAR_TRACE
  printf("In getGvar\n");
#endif
  
  rank = findGvar( Mname );
  
  if (rank>=0) 
    { /* var exists as global*/
#ifdef GVAR_TRACE
      printf("   Object found at rank %d\n", rank);
#endif

      *ptrObj = (double *)(gvarLst[rank]).ptr;
      return(0);
    }
  else
    { /* var doesn't exist as a global var */
#ifdef GVAR_TRACE
      printf("   Object not found\n");
#endif

      *ptrObj = (double *)NULL;
      return(-1); 
    }
}


/*********** rmGvar **********/
#ifndef __STDC__
int delGvar(Mname)
     Matrix *Mname;
#else /* __STDC__ */
     int delGvar( Matrix* Mname )
#endif /* __STDC__ */
     /* delete a global var */
     /* the variable memory is freed and its entry */
     /* in the global list is supressed */
     /* INPUT  : name, the name of the scilab var */
     /* OUTPUT : 0 is the operation was successful, -1 otherwise */

{
  int rank;
  
  rank = findGvar( Mname );
  if (rank>=0) 
    { /* var exists as global*/
      free ( (gvarLst[rank]).ptr );
      (gvarLst[rank]).ptr = (double *)NULL;
      free ( (gvarLst[rank]).name );
      (gvarLst[rank]).name = (double *)NULL;
      /* memcpy((gvarLst[rank]).name, (int)0, gvar_nsiz * sizeof(int)); */
      
      if (rank < curFreeElt) curFreeElt=rank;
      return(0);
    }
  else
    { /* var doesn't exist as a global var */
      /* *ptrObj = (double *)NULL */
      return(-1); 
    }
}
