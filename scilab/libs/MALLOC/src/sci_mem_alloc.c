#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sci_mem_alloc.h"

/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void * MyReAlloc(void * lpAddress,size_t dwSize,const char *fichier,int ligne)
{
  void * NewPointer=realloc(lpAddress,dwSize);
 
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
IMPORT_EXPORT_MALLOC_DLL void * MyAlloc(size_t dwSize,const char *file,int line)
{
  void * NewPointer=malloc(dwSize);

  if (dwSize>0)
    {
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
		#ifndef NDEBUG
			printf("MALLOC incorrect Size Error File %s Line %d \n",file,line);
			fflush(NULL);
		#endif
    }
  return NewPointer;
  
}

/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void * MyCalloc(size_t x, size_t y, const char *file,int line)
{
  void * NewPointer=calloc(x,y);

  if ((x)*(y)>0)
    {
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
    if(x != NULL)
	    free(x);
}
/*-----------------------------------------------------------------------------------*/
