/* Copyright INRIA/ENPC */
/*
  Interface with system C function
  */
#include<stdio.h>
#include "../machine.h"

#ifdef __STDC__
#include <stdlib.h>
#else
int  system();
#endif

int C2F(systemc)(char *command, integer *stat)
{
  int status;
#ifdef __ABSC__ 
  char * cmd = (char *) malloc((strlen(command) + 7) * sizeof(char));
  strcpy(cmd,"CMD /C ");
  strcat(cmd,command);
  status=system(cmd);
  free(cmd);
#else
  status=system(command);
#endif
  *stat=(integer)status;
  return(0);
}
