/* Copyright INRIA */

#include <windows.h> 
#include <stdio.h>

int WINAPI DllMain (HINSTANCE hInstance , 
		           DWORD reason,
		           PVOID pvReserved)
{
  switch (reason) 
    {
    case DLL_PROCESS_ATTACH:
      break;
    case DLL_PROCESS_DETACH:
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    }
  return 1;
}

int doittoo(int i)
{
      printf("In foo2.c inside of doittoo\n");
      return(i+10);
}

int doit (int i)
{
     printf("In foo.c inside of doit\n");
     return( doittoo(i) );
}

