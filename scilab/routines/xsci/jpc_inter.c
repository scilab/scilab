/* Copyright (C) 1998 Chancelier Jean-Philippe */
#ifdef SYSV
#include <string.h>
#else
#include <strings.h>
#endif
#include <stdio.h>
#include "x_ptyxP.h"
#include "x_data.h"
#include "../machine.h"
#include "All-extern-x.h"
#include "All-extern.h"
/* 
   write_scilab set a scilab command to be executed and displayed at prompt time
   works only with xsci interface
*/

static void str_to_xterm ( string, nbytes)
    register char *string;
    int nbytes;
{
    register TScreen *screen = &term->screen;
    StringInput(screen,string,strlen(string));
}

void write_scilab(s)
    char   *s;
{
    int  i;
    C2F(xscion)(&i);
    if (i==1)
	str_to_xterm(s,strlen(s));
    else
	printf("asynchronous actions are not supported with -nw option\n");
}




