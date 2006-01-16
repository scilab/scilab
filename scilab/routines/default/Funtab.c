/*-----------------------------------------------------------------------------------*/
/* Scilab function table 
 * Copyright ENPC Jean-Philippe Chancelier 
 * See Also copyright bellow */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../stack-c.h"
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#include "../machine.h"
/*-----------------------------------------------------------------------------------*/
#define NAMECODE 6
#define OK 1
#undef FAILED
#define FAILED 0
/** maximum number of entries in the htable **/
/** in fact myhcreate used a prime > MAXTAB **/
/** WARNING : MAXTAB must be chosen > 2* the number of entries in fundef **/
/** for good efficiency of the hash code **/
#define MAXTAB 900
/*-----------------------------------------------------------------------------------*/
typedef enum {
	SCIFIND, SCIENTER,SCIDELETE
} SCIACTION;

typedef struct 
{
	char *name;
	int codeI;
	int code;
	int level;
} Funcs ;

Funcs SciFuncs[]={
#include "fundef"
	{(char*) 0 ,  0,  0  ,   1},
};

typedef struct entry {
	int key[NAMECODE];
	int data;
} ENTRY;

typedef struct { 
	unsigned int   used;
	ENTRY entry;
} _ENTRY;
/*-----------------------------------------------------------------------------------*/
static _ENTRY   * htable = NULL;
static unsigned   hsize;
static unsigned   filled;
/*-----------------------------------------------------------------------------------*/
extern  int C2F(cvname) __PARAMS((integer *,char *,integer *, unsigned long int));
/*-----------------------------------------------------------------------------------*/
static int	 myhcreate(unsigned int nel);
static int 	 myhsearch(int *key, int *data, int *level, SCIACTION action);
static int Eqid(int *x, int *y);
static void  Init(void) ;
static int backsearch(int *key, int *data);
/*-----------------------------------------------------------------------------------*/
/************************************************************
 *    Hash table for scilab functions 
 *    job is used to specify a job
 *   job : indicateur  de l'operation a effectuer
 *   0 impression de la table des primitives id et fptr ne sont pas
 *           references
 *   1 recherche du pointeur fptr associe au nom id, le sous programme
 *           retourne la valeur du pointeur dans fptr. S'il n'y a pas
 *           de fonction correspondant a id funtab retourne fptr=0
 *           si id est un nom incorrect funtab retourne fptr=-1
 *   2 recherche du nom id associe au pointeur fptr s'il n'y a pas
 *           de fonction associee a ce pointeur funtab retourne
 *           id(1)=0
 *   3 ajout du nom donne par id avec le pointeur donne par fptr
 *           en cas d'erreur (nom incorrecte ou capacite de la table
 *           depasse funtab appele error et retourne avec err>0
 *   4 suppression du nom donne par id, le sous programme retourne
 *           la valeur du pointeur associe dans fptr. S'il n'y a pas
 *           de fonction correspondant a id funtab retourne fptr=0
 *           si id est un nom incorrect funtab retourne fptr=-1
 *   id  :vecteur de taille nsiz contenant le code scilab du nom
 *   fptr:entier
 ************************************************************/
int C2F(funtab)(int *id, int *fptr, int *job)
{
  int level=0, j=0;
  Init();
  switch ( *job) 
    {
    case 0 : /* print */ 
      sciprint("Internal functions: \n\n");      
      /* the scilab function what() is so far the only user of this case */
      while ( SciFuncs[j].name != (char *) 0 ) {
        sciprint("%s",SciFuncs[j].name); j++;
        if (j%5==0) sciprint("\n"); else sciprint(" ");
      }
      sciprint("\n\n");      
    break;
    case 1 :
      *fptr=0;
      myhsearch(id,fptr,&level,SCIFIND);
      break;
    case 2 : 
      backsearch(id,fptr);
      break;
    case 3 : 
      if ( myhsearch(id,fptr,&level,SCIENTER) == FAILED );
      break;
      /** XXX : appeller error **/
    case 4 : 
      myhsearch(id,fptr,&level,SCIDELETE);
      break;
    }
  return(0);
}
/*-----------------------------------------------------------------------------------*/  
static int EnterStr(char *str, int *dataI, int *data, int *level)
{
  int ldata;
  int id[NAMECODE];
  int zero=0;
  C2F(cvname)(id,str,&zero,strlen(str));
  ldata= (*dataI)*100+*data;
  return( myhsearch(id,&ldata,level,SCIENTER));
}
/*-----------------------------------------------------------------------------------*/
static void  Init(void)
{
  static int firstentry = 0;
  int j=0;
  if ( firstentry != 0 ) return;
  if ( myhcreate(MAXTAB) == 0 ) 
    {
      printf("Fatal Error : Can't create table for scilab functions \n");
      exit(1);
    }
  while ( SciFuncs[j].name != (char *) 0 )
    {
      if ( EnterStr(SciFuncs[j].name,&SciFuncs[j].codeI,&SciFuncs[j].code,
		    &SciFuncs[j].level) == FAILED)
	{
	  printf("Fatal Error : Table for scilab functions is too small \n");
	  exit(1);
	}
      j++;
    }
  firstentry = 1;
}
/*-----------------------------------------------------------------------------------*/
/************************************************
 * Hashtable code : 
 * slightly modified to add DELETE 
 * Jean-Philippe Chancelier ( Scilab Group )
 ************************************************/
/* 
 * For the used double hash method the table size has to be a prime. To
 * correct the user given table size we need a prime test.  This trivial
 * algorithm is adequate because
 * a)  the code is (most probably) only called once per program run and
 * b)  the number is small because the table must fit in the core
 */
static int isprime(unsigned int number)
{
    /* no even number will be passed */
    unsigned div = 3;

    while (div*div < number && number%div != 0)
        div += 2;

    return number%div != 0;
}
/*-----------------------------------------------------------------------------------*/
/*
 * Before using the hash table we must allocate memory for it.
 * Test for an existing table are done. We allocate one element
 * more as the found prime number says. This is done for more effective
 * indexing as explained in the comment for the hsearch function.
 * The contents of the table is zeroed, especially the field used 
 * becomes zero.
 */
static int myhcreate(unsigned int nel)
{
    /* There is still another table active. Return with error. */
    if (htable != NULL)
	return 0;

    /* Change nel to the first prime number not smaller as nel. */
    nel |= 1;      /* make odd */
    while (!isprime(nel)) nel += 2;

    hsize  = nel;
    filled = 0;
    /* printf(" Size of hTable %d\n",nel); */
    /* allocate memory and zero out */
    if ((htable = (_ENTRY *)CALLOC(hsize+1, sizeof(_ENTRY))) == NULL)
	return 0;

    /* everything went alright */
    return 1;
}
/*-----------------------------------------------------------------------------------*/
static int backsearch(int *key, int *data)
{
  unsigned int i;
  for ( i = 0 ; i < hsize ; i++ ) 
    if ( htable[i].used && htable[i].entry.data == *data ) 
      {
	int j;
	for (j = 0; j < NAMECODE ; j++ ) 
	  key[j] = htable[i].entry.key[j];
	return(1);
      }
  return(0);
}
/*-----------------------------------------------------------------------------------*/  
/*****************************************************************************
 * This is the search function. It uses double hashing with open adressing.
 * The argument item.key has to be a pointer to an zero terminated, most
 * probably strings of chars. The function for generating a number of the
 * strings is simple but fast. It can be replaced by a more complex function
 * like ajw (see [Aho,Sethi,Ullman]) if the needs are shown.
 *
 * We use an trick to speed up the lookup. The table is created by hcreate
 * with one more element available. This enables us to use the index zero
 * special. This index will never be used because we store the first hash
 * index in the field used where zero means not used. Every other value
 * means used. The used field can be used as a first fast comparison for
 * equality of the stored and the parameter value. This helps to prevent
 * unnecessary expensive calls of strcmp.
 ******************************************************************************/
static int myhsearch(int *key, int *data, int *level, SCIACTION action)
{
  register unsigned hval;
  register unsigned hval2;
  register unsigned count;
  register unsigned len = NAMECODE;
  register unsigned idx;

    /*
     * If table is full and another entry should be entered return with 
     * error.
     */

    if (action == SCIENTER && filled == hsize) 
        return FAILED;

    /* Compute a value for the given string. Perhaps use a better method. */
    hval  = len;
    count = len;
    while (count-- > 0) {
      hval += key[count];
      hval %= hsize;
    }

    /* First hash function: simply take the modul but prevent zero. */

    if (hval == 0) hval++;

    /* The first index tried. */
    idx = hval;

    if (htable[idx].used) 
      {
	/* Further action might be required according to the action value. */
	
	if (htable[idx].used == hval )
	  {
	    if ( Eqid(key, htable[idx].entry.key) == 0) 
	      {
		switch (action) 
		  {
		  case SCIDELETE :
		    htable[idx].used = 0;
		    filled--;
		    return OK ;
		    break;
		  case SCIENTER :
		    htable[idx].entry.data = *data; 
		    return OK;
		  case SCIFIND :
		    *data = htable[idx].entry.data;
		    return OK;
		  }
	      }
	  }
	
	/* Second hash function, as suggested in [Knuth] */

        hval2 = 1 + hval % (hsize-2);
	
        do {
	    /* 
	     * Because hsize is prime this guarantees to step through all
             * available indeces.
	     */
            if (idx <= hval2)
	        idx = hsize+idx-hval2;
	    else
	        idx -= hval2;

            /* If entry is found use it. */
            if (htable[idx].used == hval ) 
	      {
                if ( Eqid(key, htable[idx].entry.key) == 0) 
		  {
		    switch (action) 
		      {
		      case SCIDELETE :
			htable[idx].used = 0;
			filled--;
			return OK;
			break;
		      case SCIENTER :
			htable[idx].entry.data = *data; 
			return OK;
		      case SCIFIND :
			*data = htable[idx].entry.data; 
			return OK;
		      }
		  }
	      }
	  } while (htable[idx].used);
      }
    
    /* An empty bucket has been found. */

    if (action == SCIENTER) 
      {
	int i;
        htable[idx].used  = hval;
	for ( i=0 ; i < NAMECODE ; i++ )
	  htable[idx].entry.key[i] = key[i];
        htable[idx].entry.data = *data;
	filled++;
        return OK ;
      }
    else
      return FAILED;
}
/*-----------------------------------------------------------------------------------*/  
static int Eqid(int *x, int *y)
{
  int i;
  for (i = 0; i < NAMECODE ; i++ ) 
    if ( x[i] != y[i] ) 
      return(1);
  return(0);
}
/*-----------------------------------------------------------------------------------*/  
