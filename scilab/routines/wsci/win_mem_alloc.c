#include "win_mem_alloc.h"
/* Allan CORNET */
/* INRIA 2005 */

#include <memory.h>

LPVOID VirtualReAlloc(LPVOID lpAddress,SIZE_T dwSize)
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
	NewPointer=VirtualAlloc(NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);
 }

 return NewPointer;
}