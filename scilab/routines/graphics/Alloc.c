/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
 * Allocation routines for working arrays 
 ------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include "Math.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


/*--------------------------------------------
 * void graphic_alloc_info() 
 * void graphic_alloc_free()
 * void * graphic_alloc(indice,n,size)
 *
 * maintains a set of S_alloc_max working arrays 
 * Usage : 
 *   int *x = graphic_alloc(0,10,sizeof(int)) 
 *   double *y = graphic_alloc(1,100,sizeof(double))
 --------------------------------------------*/

#define NBPOINTS 256 

typedef struct s_alloc { 
  int init;          /* used for first allocation check */
  unsigned int size; /* currently allocated space in bytes */
  void *storage;     /* pointer to allocated space */
} S_alloc;

#define S_alloc_max 9 

static S_alloc Storage[S_alloc_max] = { {0,0,NULL},{0,0,NULL},{0,0,NULL},
					{0,0,NULL},{0,0,NULL},{0,0,NULL},
					{0,0,NULL},{0,0,NULL},{0,0,NULL}};

void * graphic_alloc(int indice, int n, unsigned int size)
{
  integer *p; 
  unsigned int size_needed = n*size;
  unsigned int block_size = Storage[indice].size;
  /* check indice */
  if ( indice < 0 || indice >= S_alloc_max ) return 0;
  if ( n == 0 ) return 0;
  if (size_needed <= block_size  ) 
    /* no need to alloc or realloc */
    return Storage[indice].storage;
  /* compute size to be dynamically allocated */
  while ( size_needed > block_size ) block_size += NBPOINTS ;
  if ( Storage[indice].init == 0) 
    /** Allocation **/
    { p = (integer*) MALLOC( block_size); }
  else
    /** Reallocation **/
    { p = (integer *)  REALLOC( Storage[indice].storage,block_size ) ; }
  if ( p == NULL) 
    return 0;
  Storage[indice].storage= p ;
  Storage[indice].init = 1;
  Storage[indice].size = block_size;
  return Storage[indice].storage;
}

void graphic_alloc_info(void)
{
  unsigned int gsize=0;
  int i;
  for (i = 0 ; i < S_alloc_max ; i++) 
    if ( Storage[i].init == 1 ) gsize += Storage[i].size;
  sciprint("Graphic allocated dynamic memory: %ud bytes\n",gsize);
}

void graphic_alloc_free(void)
{
  int i;
  for (i = 0 ; i < S_alloc_max ; i++) 
    if ( Storage[i].init == 1 ) 
      {
	FREE(Storage[i].storage);
	Storage[i].init = 0;
      }
}

