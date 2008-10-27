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
#include "core_math.h"
#include "inffic.h"
#include "scilabDefaults.h"
#define MAXDATA 5 



static char *dataStrings[] = {
  "$MANCHAPTERS",
  /* start_up instructions file */
  "exec('SCI/etc/scilab.start',-1);",
  /* demos instructions file */
  "scilab_demos();",	 
  /* on crash save file */
  "home/scilab.save",
  /* exit instructions file */
  "exec('SCI/etc/scilab.quit',-1);quit;"	 ,
  /* on exit , we try to catch errors */
  /* workaround to not have scilab "zombie" */
  /* bug 3672 */
  "exec('SCI/etc/scilab.quit','errcatch',-1);quit;"	 
};

/******************************************
 * get string associated with iopt 
 ******************************************/

void C2F(inffic)(int *iopt, char *name, int *nc)
{
  *nc=(int) strlen(dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
  strcpy(name,dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
}

/******************************************
 * just get string length 
 ******************************************/

void C2F(infficl)(int *iopt, int *nc)
{
  *nc=(int) strlen(dataStrings[Max(Min(*iopt-1,MAXDATA),0)]);
}


/******************************************
 *  get string .....
 ******************************************/

char *get_sci_data_strings(int n)
{
  return dataStrings[Max(Min(n,MAXDATA),0)];
}


