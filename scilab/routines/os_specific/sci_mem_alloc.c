#include "../sci_mem_alloc.h" /* For now, its header is in SCI/routines */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Allan CORNET */
/* INRIA 2005 */

#ifndef NULL
#define NULL 0
#endif

/*-----------------------------------------------------------------------------------*/
void * MyReAlloc(void * lpAddress,int dwSize,char *fichier,int ligne)
{
  
  void * NewPointer=NULL;
  
  NewPointer = realloc(lpAddress,dwSize);
  
  if(NewPointer == NULL)
    {
#ifndef NDEBUG
      printf("REALLOC Error File %s Line %d \n",fichier,ligne);
      fflush(NULL);
      /* exit(1); */
#endif
    }
  
  return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
void * MyAlloc(unsigned int dwSize,char *fichier,int ligne)
{
  void * NewPointer=NULL;
  
  if (dwSize>0)
    {
      NewPointer = malloc(dwSize);
      
      if (NewPointer == NULL)
	{
#ifndef NDEBUG
	  printf("MALLOC Error File %s Line %d \n",fichier,ligne);
	  fflush(NULL);
	  /* exit(1); */
#endif
	}
    }
  else
    {
#ifndef NDEBUG
      printf("MALLOC Error File %s Line %d \n",fichier,ligne);
      fflush(NULL);
      /* exit(1); */
#endif
    }
  
  return NewPointer;
  
}


/*-----------------------------------------------------------------------------------*/
void * MyCalloc(unsigned int x, unsigned int y, char *fichier,int ligne)
{
  void * NewPointer=NULL;
  
  if ((x)*(y)>0)
    {
      NewPointer = calloc(x,y);
      
      if (NewPointer == NULL)
	{
#ifndef NDEBUG
	  printf("CALLOC Error File %s Line %d \n",fichier,ligne);
	  fflush(NULL);
	  /* exit(1); */
#endif
	}
    }
  else
    {
#ifndef NDEBUG
	  printf("CALLOC Error File %s Line %d \n",fichier,ligne);
	  fflush(NULL);
	  /* exit(1); */
#endif
    }

  return NewPointer;

}
/*-----------------------------------------------------------------------------------*/
