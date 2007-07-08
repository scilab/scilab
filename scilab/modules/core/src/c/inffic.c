/* Copyright INRIA/ENPC */
#include <string.h>
#include "machine.h"
#include "core_math.h"
#include "inffic.h"

#define MAXDATA 5 



static char *dataStrings[] = {
  "$MANCHAPTERS",
  "exec('SCI/etc/scilab.start',-1);",         /* start_up instructions file   */
  "scilab_demos();",	 /* demos instructions file      */
#ifdef _MSC_VER
  "home/history.scilab",		 /* history file                */
#else 
  "home/.history.scilab",		 /* history file                */
#endif
  "home/scilab.save",			 /* on crash save file          */
  "exec('SCI/etc/scilab.quit',-1);quit;"	 /* exit instructions file      */
};

/******************************************
 * get string associated with iopt 
 ******************************************/

void C2F(inffic)(integer *iopt, char *name, integer *nc)
{
  *nc=strlen(dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
  strcpy(name,dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
}

/******************************************
 * just get string length 
 ******************************************/

void C2F(infficl)(integer *iopt, integer *nc)
{
  *nc=strlen(dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
}


/******************************************
 *  get string .....
 ******************************************/

char *get_sci_data_strings(int n)
{
  return dataStrings[Max(Min(n,MAXDATA),0)];
}


