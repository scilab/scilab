/* Copyright INRIA/ENPC */

#include "../machine.h"
#include <signal.h>

extern int   C2F(sigbas)(); /*  __PARAMS((integer *));*/

void controlC_handler (int sig)
{
  int j = SIGINT;
  C2F(sigbas)(&j);
}

int C2F(csignal)(void)
{
  signal (SIGINT, controlC_handler);
  return(0);
}

