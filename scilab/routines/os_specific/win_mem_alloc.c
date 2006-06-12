#include "win_mem_alloc.h"
/* Allan CORNET */
/* INRIA 2005 */
#include <stdio.h>
#include <string.h>
#include <memory.h>
/*-----------------------------------------------------------------------------------*/
#define MEMDISPO (MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN)
/*-----------------------------------------------------------------------------------*/
IMPORT_DLL int IsFromC(void);
/* When you call scilab dynamically (LoadLibrary,FreeLibrary) (DLL) don't use HeapAlloc or VirtualAlloc
  problem with process caller
*/
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL LPVOID MyHeapRealloc(LPVOID lpAddress,SIZE_T dwSize,char *fichier,int ligne)
{

 LPVOID NewPointer=NULL;
 SIZE_T precSize=0;

 if (lpAddress)
 {
	 if ( IsFromC() )
	 {
		 NewPointer=realloc(lpAddress,dwSize);
	 }
	 else
	 {
		 NewPointer=HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,lpAddress,dwSize);
	 }
 }
 else
 {
	#ifdef _DEBUG
		char MsgError[1024];
		wsprintf(MsgError,"REALLOC (1) Error File %s Line %d ",fichier,ligne);
		MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
	#endif
		if ( IsFromC() )
		{
			NewPointer=malloc(dwSize);
		}
		else
		{
			NewPointer=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}

	if (NewPointer==NULL)
	{
		#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"REALLOC (2) Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
		#endif
	}
 }
 return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL LPVOID MyHeapAlloc(SIZE_T dwSize,char *fichier,int ligne)
{
	LPVOID NewPointer=NULL;

	if (dwSize>0)
	{
		if ( IsFromC() )
		{
			NewPointer=malloc(dwSize);
		}
		else
		{
			NewPointer=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}

		if (NewPointer==NULL)
		{
			#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"MALLOC (1) Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
			#endif
		}
	}
	else
	{
			#ifdef _DEBUG
				char MsgError[1024];
				wsprintf(MsgError,"MALLOC (2) Error File %s Line %d ",fichier,ligne);
				MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
			#endif
				if ( IsFromC() )
				{
					NewPointer=malloc(dwSize);
				}
				else
				{
					NewPointer=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
				}
	}
	return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void MyHeapFree(LPVOID lpAddress,char *fichier,int ligne)
{
	if ( IsFromC() )
	{
		free(lpAddress);
	}
	else
	{
		HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,lpAddress);
	}
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL LPVOID MyVirtualAlloc(SIZE_T dwSize,char *fichier,int ligne)
{
	LPVOID NewPointer=NULL;

	if (dwSize>0)
	{
		if ( IsFromC() )
		{
			NewPointer=malloc(dwSize);
		}
		else
		{
			NewPointer=VirtualAlloc(NULL,((unsigned) dwSize),MEMDISPO,PAGE_READWRITE);
		}

		if (NewPointer==NULL)
		{
			#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"MALLOC (VirtualAlloc 1) Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
			#endif
		}
	}
	else
	{
		#ifdef _DEBUG
		char MsgError[1024];
		wsprintf(MsgError,"MALLOC (VirtualAlloc 2) Error File %s Line %d ",fichier,ligne);
		MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
		#endif

		if ( IsFromC() )
		{
			NewPointer=malloc(dwSize);
		}
		else
		{
			NewPointer=VirtualAlloc(NULL,((unsigned) dwSize),MEMDISPO,PAGE_READWRITE);
		}
	}

	return NewPointer;

}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void MyVirtualFree(LPVOID lpAddress,char *fichier,int ligne)
{
	if ( IsFromC() )
	{
		free(lpAddress);
	}
	else
	{
		VirtualFree(lpAddress,0,MEM_RELEASE);
	}
}
/*-----------------------------------------------------------------------------------*/
