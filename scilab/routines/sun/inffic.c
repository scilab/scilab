/* Copyright INRIA/ENPC */
#include <string.h>

#if defined(THINK_C)||defined(__MWERKS__)
#include "::graphics:Math.h"
#else
#include "../graphics/Math.h"
#endif

#define MAXDATA 5 

static char *dataStrings[] = {
  "$MANCHAPTERS",
  "exec('SCI/scilab.star',-1);",         /* start_up instructions file   */
  "exec('SCI/demos/alldems.dem');",	 /* demos instructions file      */
  "home/scilab.hist",			 /* history file                */
  "home/scilab.save",			 /* on crash save file          */
  "exec('SCI/scilab.quit',-1);quit;"	 /* exit instructions file      */
};

/******************************************
 * get string associated with iopt 
 ******************************************/

void C2F(inffic)(iopt,name,nc)
     integer *iopt,*nc;
     char *name;
{
  *nc=strlen(dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
  strcpy(name,dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
}

/******************************************
 * just get string length 
 ******************************************/

void C2F(infficl)(iopt,nc)
     integer *iopt,*nc;
{
  *nc=strlen(dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
}

