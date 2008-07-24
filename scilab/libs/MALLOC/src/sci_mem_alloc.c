#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sci_mem_alloc.h"

/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void * MyReAlloc(void * lpAddress,int dwSize,const char *fichier,int ligne)
{
  void * NewPointer=NULL;
 
  NewPointer = realloc(lpAddress,dwSize);
  
  if(NewPointer == NULL)
    {
	#ifndef NDEBUG
      printf("REALLOC returns NULL Error File %s Line %d \n",fichier,ligne);
      fflush(NULL);
	#endif
    }
  return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void * MyAlloc(unsigned int dwSize,const char *file,int line)
{
  void * NewPointer=NULL;

  if (dwSize>0)
    {
      NewPointer = malloc(dwSize);
      
      if (NewPointer == NULL)
		{
		#ifndef NDEBUG
			printf("MALLOC returns NULL Error File %s Line %d \n",file,line);
			fflush(NULL);
		#endif
		}
    }
  else
    { 
		NewPointer = malloc(dwSize);
		#ifndef NDEBUG
			printf("MALLOC incorrect Size Error File %s Line %d \n",file,line);
			fflush(NULL);
		#endif
    }
  return NewPointer;
  
}

/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void * MyCalloc(unsigned int x, unsigned int y, const char *file,int line)
{
  void * NewPointer=NULL;

  if ((x)*(y)>0)
    {
		NewPointer = calloc(x,y);
      
		if (NewPointer == NULL)
		{
		#ifndef NDEBUG
			printf("CALLOC returns NULL Error File %s Line %d \n",file,line);
			fflush(NULL);
		#endif
		}
    }
  else
    {
		NewPointer = calloc(x,y);
		#ifndef NDEBUG
			printf("CALLOC incorrect size Error File %s Line %d \n",file,line);
			fflush(NULL);
		#endif
    }
  return NewPointer;

}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void MyFree(void *x, const char *file,int line)
{
	free((void*)x);
}
/*-----------------------------------------------------------------------------------*/
