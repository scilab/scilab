/*-------------------------------------------------------
 * Scilab function table 
 * Copyright ENPC Jean-Philippe Chancelier 
 * See Also copyright bellow 
 *-------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../machine.h"

extern  int C2F(cvname) __PARAMS((integer *,char *,integer *, unsigned long int));

/** size of name code in scilab int id[NAMECODE] */

#define NAMECODE 6
#define OK 1
#define FAILED 0

/** maximum number of entries in the htable **/
/** in fact myhcreate used a prime > MAXTAB **/
/** WARNING : MAXTAB must be chosen > 2* the number of entries in fundef **/
/** for good efficiency of the hash code **/

#define MAXTAB 900

typedef enum {
  FIND, ENTER,DELETE
} ACTION;


static int	 myhcreate(unsigned int nel);
/* static void	 myhdestroy(); */
static int 	 myhsearch(int *key, int *data, int *level, ACTION action);
static int Eqid(int *x, int *y);
static void  Init(void) ;
static int backsearch(int *key, int *data);

#ifdef TEST 
static  int C2F(cvname) __PARAMS((integer *id, char *str, integer *job,long int str_len));

static int EnterStr();
static void DeleteStr();
static int FindStr();
#endif 


/**********************
 * Scilab functions 
 **********************/

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
  int level=0;
  Init();
  switch ( *job) 
    {
    case 0 : /* print */ break;
    case 1 :
      *fptr=0;
      myhsearch(id,fptr,&level,FIND);
      break;
    case 2 : 
      backsearch(id,fptr);
      break;
    case 3 : 
      if ( myhsearch(id,fptr,&level,ENTER) == FAILED );
      break;
      /** XXX : appeller error **/
    case 4 : 
      myhsearch(id,fptr,&level,DELETE);
      break;
    }
  return(0);
}

#ifdef TEST /********************* test part ***/

main()
{
  test_hash();
}

int test_hash()
{
  int j=0,code,level,k;
  Init();
  j=0;
  while ( SciFuncs[j].name != (char *) 0 )
    {
      if ( FindStr(SciFuncs[j].name,&code,&level) == FAILED )
	{
	  printf(" %s not found \n",SciFuncs[j].name);
	}
      else
	{
	  printf(" %s found %d \n",SciFuncs[j].name,code);
	}
      j++;
    }
  for ( k = 1; k < 2 ; k++) 
    {
      printf("Je fais des delete \n");
      j=0;
      while ( SciFuncs[j].name != (char *) 0 && j < 20 )
	{
	  DeleteStr(SciFuncs[j].name,&code,&level);
	  j++;
	}
      j=0;
      printf("Je test \n");
      while ( SciFuncs[j].name != (char *) 0 && j < 20  )
	{
	  if ( FindStr(SciFuncs[j].name,&code,&level) == FAILED)
	    {
	      printf(" %s not found \n",SciFuncs[j].name);
	    }
	  else
	    {
	      printf(" %s found %d \n",SciFuncs[j].name,code);
	    }
	  j++;
	}
      printf("Je remet les fonctions \n");
      j=0;
      while ( SciFuncs[j].name != (char *) 0 && j < 20 )
	{
	  EnterStr(SciFuncs[j].name,&SciFuncs[j].codeI,&SciFuncs[j].code,
		   &SciFuncs[j].level);
	  j++;
	}
      j=0;
      while ( SciFuncs[j].name != (char *) 0 && j < 20  )
	{
	  if ( FindStr(SciFuncs[j].name,&code,&level) == FAILED)
	    {
	      printf(" %s not found \n",SciFuncs[j].name);
	    }
	  else
	    {
	      printf(" %s found %d \n",SciFuncs[j].name,code);
	    }
	  j++;
	}
    }
  return(0);
}

C2F(cvname)(id,str,n1,n2)
     int id[NAMECODE];
     char str[];
     int *n1;
     long int n2;
{
  int i,j;
  for ( i = 0; i < NAMECODE ; i++) 
    {
      id[i]=0;
      for ( j = 0; j < 4 && j+4*i < n2 ; j++) 
	id[i] = 256*id[i] + str[j+4*i];
    }
}


static void DeleteStr(str,data,level)
     char *str;
     int  *data,*level;
{
  int id[NAMECODE];
  int zero=0;
  C2F(cvname)(id,str,&zero,strlen(str));
  myhsearch(id,data,level,DELETE);
}

static int FindStr(str,data,level)
     char *str;
     int  *data,*level;
{
  int id[NAMECODE];
  int zero=0;
  C2F(cvname)(id,str,&zero,strlen(str));
  return( myhsearch(id,data,level,FIND));
}


#endif  /********************* end of test part ***/
  
static int EnterStr(char *str, int *dataI, int *data, int *level)
{
  int ldata;
  int id[NAMECODE];
  int zero=0;
  C2F(cvname)(id,str,&zero,strlen(str));
  ldata= (*dataI)*100+*data;
  return( myhsearch(id,&ldata,level,ENTER));
}

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



/************************************************
 * Hashtable code : 
 * slightly modified to add DELETE 
 * Jean-Philippe Chancelier ( Scilab Group )
 ************************************************/

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Margo Seltzer.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)search.h	8.1 (Berkeley) 6/4/93
 */


/* Backward compatibility to hsearch interface. */

typedef struct entry {
  int key[NAMECODE];
  int data;
} ENTRY;

/* Copyright (C) 1993 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@ira.uka.de>

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */


/*
 * [Aho,Sethi,Ullman] Compilers: Principles, Techniques and Tools, 1986
 * [Knuth]            The Art of Computer Programming, part 3 (6.4)
 */


/*
 * We need a local static variable which contains the pointer to the
 * allocated memory for the hash table. An entry in this table contains
 * an ENTRY and a flag for usage.
 */

typedef struct { 
    unsigned int   used;
    ENTRY entry;
} _ENTRY;

static _ENTRY   * htable = NULL;
static unsigned   hsize;
static unsigned   filled;


/* 
 * For the used double hash method the table size has to be a prime. To
 * correct the user given table size we need a prime test.  This trivial
 * algorithm is adequate because
 * a)  the code is (most probably) only called once per program run and
 * b)  the number is small because the table must fit in the core
 */

static int
isprime(unsigned int number)
{
    /* no even number will be passed */
    unsigned div = 3;

    while (div*div < number && number%div != 0)
        div += 2;

    return number%div != 0;
}

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
    if ((htable = (_ENTRY *)calloc(hsize+1, sizeof(_ENTRY))) == NULL)
	return 0;

    /* everything went alright */
    return 1;
}


/*
 * After using the hash table it has to be destroyed. The used memory can
 * be freed and the local static variable can be marked as not used.
 */
/** Unused : cleaned at scilab exit 
static void
myhdestroy()
{
    / * free used memory * /
    free(htable);

    / * the sign for an existing table is a value != NULL in htable * / 
    htable = NULL;
}
**/

/** from data to key **/

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


static int myhsearch(int *key, int *data, int *level, ACTION action)
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

    if (action == ENTER && filled == hsize) 
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
		  case DELETE :
		    htable[idx].used = 0;
		    filled--;
		    return OK ;
		    break;
		  case ENTER :
		    htable[idx].entry.data = *data; 
		    return OK;
		  case FIND :
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
		      case DELETE :
			htable[idx].used = 0;
			filled--;
			return OK;
			break;
		      case ENTER :
			htable[idx].entry.data = *data; 
			return OK;
		      case FIND :
			*data = htable[idx].entry.data; 
			return OK;
		      }
		  }
	      }
	  } while (htable[idx].used);
      }
    
    /* An empty bucket has been found. */

    if (action == ENTER) 
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


static int Eqid(int *x, int *y)
{
  int i;
  for (i = 0; i < NAMECODE ; i++ ) 
    if ( x[i] != y[i] ) 
      return(1);
  return(0);
}

