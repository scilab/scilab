/* Copyright ENPC */

#include <stdio.h>
#include "../machine.h"


#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>


main(argc, argv)
     int argc;
     char **argv;
{
  mbar_();
  TestPrintDlg();
  TestChoose();
  TestMessage();
  TestDialog() ;
  TestmDialogWindow();
  TestMatrixDialogWindow();
  TestChoice();
  return(0);
}



#define PROMPT "-->"

F2C(scilab)(nostartup)
     int *nostartup;
{

}

F2C(dsort)(){};

C2F(sigbas)(i)
     int *i;
{
  fprintf(stderr,"CTRL_C activated \n");
}

#define IP0 (int *) 0

check_win()
{
  int verb=0,win,na,v;
  C2F(dr1)("xget","window",&verb,&win,&na,v,v,v,0,0);
  C2F(dr1)("xset","window",&win,IP0,IP0,IP0,IP0,IP0,0,0);
}


int C2F(scilines)(nl,nc)
     int *nl, *nc;
{}

int C2F(sciquit)(nl,nc)
     int *nl, *nc;
{return(1);}
     

void cerro(str)
char *str;
{
  fprintf(stderr,"%s",str);
}

void cout(str)
char *str;
{
  fprintf(stdout,"%s",str);
}

void cvstr_(){}


