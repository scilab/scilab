/* Allan CORNET */
/* INRIA 2005 */
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "../includes/win_mem_alloc.h"
/*-----------------------------------------------------------------------------------*/
/* an interesting article about HeapAlloc,malloc, and OctAlloc */
/* bench show that HeapAlloc is faster than malloc on Windows */
/* http://denisbider.blogspot.com/2007/10/heap-allocation-on-multi-core-systems.html */
/*-----------------------------------------------------------------------------------*/
#define FREE_FLAGS 0
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL LPVOID MyHeapRealloc(LPVOID lpAddress,SIZE_T dwSize,char *fichier,int ligne)
{
	LPVOID NewPointer = NULL;
	SIZE_T precSize = 0;

	if (lpAddress)
	{
		_try
		{
			NewPointer = HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,lpAddress,dwSize);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}
	else
	{
		NewPointer = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);

		if (NewPointer == NULL)
		{
			#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"REALLOC (1) Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
			#endif
		}
	}
	return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL LPVOID MyHeapAlloc(SIZE_T dwSize,char *fichier,int ligne)
{
	LPVOID NewPointer = NULL;

	if (dwSize>0)
	{
		_try
		{
			NewPointer = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

		if (NewPointer == NULL)
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
		_try
		{
			NewPointer = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}
	return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void MyHeapFree(LPVOID lpAddress,char *fichier,int ligne)
{
	_try
	{
		HeapFree(GetProcessHeap(),FREE_FLAGS,lpAddress);
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{
		#ifdef _DEBUG
		char MsgError[1024];
		wsprintf(MsgError,"FREE Error File %s Line %d ",fichier,ligne);
		MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
		#endif
	}
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL LPVOID MyVirtualAlloc(SIZE_T dwSize,char *fichier,int ligne)
{
	LPVOID NewPointer=NULL;

	if (dwSize>0)
	{
		_try
		{
			NewPointer = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
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

		_try
		{
			NewPointer = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

	}

	return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void MyVirtualFree(LPVOID lpAddress,char *fichier,int ligne)
{
	if (lpAddress) 
	{
		_try
		{
			HeapFree(GetProcessHeap(),FREE_FLAGS,lpAddress);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}
}
/*-----------------------------------------------------------------------------------*/
