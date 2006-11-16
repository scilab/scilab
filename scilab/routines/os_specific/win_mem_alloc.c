#include "win_mem_alloc.h"
/* Allan CORNET */
/* INRIA 2005 */
#include <stdio.h>
#include <string.h>
#include <memory.h>
/*-----------------------------------------------------------------------------------*/
#define MEMDISPO (MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN)
/*-----------------------------------------------------------------------------------*/
extern int IsFromC(void);
/* When you call scilab dynamically (LoadLibrary,FreeLibrary) (DLL) don't use HeapAlloc or VirtualAlloc
  problem with process caller
*/
/*-----------------------------------------------------------------------------------*/
LPVOID MyHeapRealloc(LPVOID lpAddress,SIZE_T dwSize,char *fichier,int ligne)
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
		//exit(1);
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
				if ( IsFromC() )
				{
					NewPointer=malloc(dwSize);
				}
				else
				{
					NewPointer=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
				}
	}
    //printf("MyHeapAlloc %d %s %d\n",NewPointer,fichier,ligne);
	return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
void MyHeapFree(LPVOID lpAddress,char *fichier,int ligne)
{
	if ( IsFromC() )
	{
		_try
		{
			free(lpAddress);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}
	else
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
	}
	 //printf("MyHeapFree %d %s %d\n",lpAddress,fichier,ligne);
}
/*-----------------------------------------------------------------------------------*/
LPVOID MyVirtualAlloc(SIZE_T dwSize,char *fichier,int ligne)
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

		if ( IsFromC() )
		{
			NewPointer=malloc(dwSize);
		}
		else
		{
			NewPointer=VirtualAlloc(NULL,((unsigned) dwSize),MEMDISPO,PAGE_READWRITE);
		}
	}

	//printf("MyVirtualAlloc %d %s %d\n",NewPointer,fichier,ligne);
	return NewPointer;

}
/*-----------------------------------------------------------------------------------*/
void MyVirtualFree(LPVOID lpAddress,char *fichier,int ligne)
{
	if ( IsFromC() )
	{
		free(lpAddress);
	}
	else
	{
		VirtualFree(lpAddress,0,MEM_RELEASE);
	}
	//printf("MyVirtualFree %d %s %d\n",lpAddress,fichier,ligne);
}
/*-----------------------------------------------------------------------------------*/
