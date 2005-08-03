#include "win_mem_alloc.h"
/* Allan CORNET */
/* INRIA 2005 */

#include <memory.h>
/*-----------------------------------------------------------------------------------*/
LPVOID VirtualReAlloc(LPVOID lpAddress,SIZE_T dwSize,char *fichier,int ligne)
{

 LPVOID NewPointer=NULL;
 SIZE_T precSize=0;

 if (lpAddress)
 {
	 precSize=sizeof(lpAddress);
	 NewPointer=VirtualAlloc(NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);
	 memcpy(NewPointer,lpAddress,precSize);
	 VirtualFree((char *)lpAddress, 0, MEM_RELEASE);
 }
 else
 {
	#ifdef _DEBUG
		char MsgError[1024];
		wsprintf(MsgError,"REALLOC Error File %s Line %d ",fichier,ligne);
		MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
		exit(1);
	#endif

	NewPointer=VirtualAlloc(NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);
	if (NewPointer==NULL)
	{
		#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"REALLOC Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
			exit(1);
		#endif
	}
 }

 return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
LPVOID MyVirtualAlloc(SIZE_T dwSize,char *fichier,int ligne)
{
	LPVOID NewPointer=NULL;

	if (dwSize>0)
	{
		NewPointer=VirtualAlloc(NULL,((unsigned) dwSize),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
		if (NewPointer==NULL)
		{
			#ifdef _DEBUG
			char MsgError[1024];
			wsprintf(MsgError,"MALLOC Error File %s Line %d ",fichier,ligne);
			MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
			exit(1);
			#endif
		}
	}
	else
	{
			#ifdef _DEBUG
				char MsgError[1024];
				wsprintf(MsgError,"MALLOC Error File %s Line %d ",fichier,ligne);
				MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
				exit(1);
			#endif
	}

	return NewPointer;

}
/*-----------------------------------------------------------------------------------*/