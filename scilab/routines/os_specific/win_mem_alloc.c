#include "win_mem_alloc.h"
/* Allan CORNET */
/* INRIA 2005 */
#include <stdio.h>
#include <string.h>
#include <memory.h>
/*-----------------------------------------------------------------------------------*/
#define MEMDISPO (MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN)
/*-----------------------------------------------------------------------------------*/
LPVOID MyHeapRealloc(LPVOID lpAddress,SIZE_T dwSize,char *fichier,int ligne)
{

 LPVOID NewPointer=NULL;
 SIZE_T precSize=0;

 if (lpAddress)
 {
	_try
	{
		NewPointer=HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,lpAddress,dwSize);
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

 }
 else
 {
	#ifdef _DEBUG
		char MsgError[1024];
		wsprintf(MsgError,"REALLOC (1) Error File %s Line %d ",fichier,ligne);
		MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
		//exit(1);
	#endif
		_try
		{
			NewPointer=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}



	if (NewPointer==NULL)
	{
		#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"REALLOC (2) Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
			//exit(1);
		#endif
	}
 }
 //printf("MyHeapReAlloc %d %s %d\n",NewPointer,fichier,ligne);
 return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
LPVOID MyHeapAlloc(SIZE_T dwSize,char *fichier,int ligne)
{
	LPVOID NewPointer=NULL;

	if (dwSize>0)
	{
		_try
		{
			NewPointer=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

		if (NewPointer==NULL)
		{
			#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"MALLOC (1) Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
			//exit(1);
			#endif
		}
	}
	else
	{
	#ifdef _DEBUG
		char MsgError[1024];
		wsprintf(MsgError,"MALLOC (2) Error File %s Line %d ",fichier,ligne);
		MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
		//exit(1);
	#endif
		_try
		{
			NewPointer=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}


	}
    //printf("MyHeapAlloc %d %s %d\n",NewPointer,fichier,ligne);
	return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
void MyHeapFree(LPVOID lpAddress,char *fichier,int ligne)
{
	_try
	{
		HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,lpAddress);
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{
		#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"FREE Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
		#endif
	}
	 //printf("MyHeapFree %d %s %d\n",lpAddress,fichier,ligne);
}
/*-----------------------------------------------------------------------------------*/
LPVOID MyVirtualAlloc(SIZE_T dwSize,char *fichier,int ligne)
{
	LPVOID NewPointer=NULL;

	if (dwSize>0)
	{
		_try
		{
			NewPointer=VirtualAlloc(NULL,((unsigned) dwSize),MEMDISPO,PAGE_READWRITE);
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
			//exit(1);
			#endif
		}
	}
	else
	{
		#ifdef _DEBUG
		char MsgError[1024];
		wsprintf(MsgError,"MALLOC (VirtualAlloc 2) Error File %s Line %d ",fichier,ligne);
		MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
		//exit(1);
		#endif

		_try
		{
			NewPointer=VirtualAlloc(NULL,((unsigned) dwSize),MEMDISPO,PAGE_READWRITE);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}

	}

	//printf("MyVirtualAlloc %d %s %d\n",NewPointer,fichier,ligne);
	return NewPointer;

}
/*-----------------------------------------------------------------------------------*/
void MyVirtualFree(LPVOID lpAddress,char *fichier,int ligne)
{
	if (lpAddress) 
	{
		_try
		{
			VirtualFree(lpAddress,0,MEM_RELEASE);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

}
/*-----------------------------------------------------------------------------------*/
