/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "machine.h"
#include "core_math.h"
#include "inffic.h"
#include "scilabDefaults.h"
#define MAXDATA 4 



static char *dataStrings[] = {
  "$MANCHAPTERS",
  "exec('SCI/etc/scilab.start',-1);",         /* start_up instructions file   */
  "scilab_demos();",	 /* demos instructions file      */
  "home/scilab.save",			 /* on crash save file          */
  "exec('SCI/etc/scilab.quit',-1);quit;"	 /* exit instructions file      */
};

/******************************************
 * get string associated with iopt 
 ******************************************/

void C2F(inffic)(integer *iopt, char *name, integer *nc)
{
  *nc=(integer)strlen(dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
  strcpy(name,dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
}

/******************************************
 * just get string length 
 ******************************************/

void C2F(infficl)(integer *iopt, integer *nc)
{
  *nc=(integer)strlen(dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
}


/******************************************
 *  get string .....
 ******************************************/

char *get_sci_data_strings(int n)
{
  return dataStrings[Max(Min(n,MAXDATA),0)];
}


