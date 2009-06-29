/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "machine.h"
#include "MALLOC.h"
#include "intcscicos.h"
#include "scoMisc.h"
#undef Top
#include "stack-c.h"
#define Top C2F(vstk).top
#include "BuildObjects.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "HandleManagement.h" /* sciGetPointerFromHandle */
#include "cvstr.h"
#include "sciprint.h"
#include "libinter.h"
#include "scicos-def.h"
#include "Scierror.h"
#include "tree.h"
#include "scicos.h"
#include "import.h" /* getscicosvarsfromimport */
#include "BOOL.h"
#include "il_state.h"
#include "il_sim.h"
/*--------------------------------------------------------------------------*/ 
extern int *listentry(int *header, int i); /* mexlib.h */
extern void str2sci(char** x,int n,int m); /* core/src/c/str2sci.h */
extern void C2F(itosci)();
extern int C2F(scierr)();
extern void C2F(vvtosci)();
extern int C2F(mktlist)(); 
extern int C2F(namstr)();
extern int C2F(funnum)(char *fname);
/*--------------------------------------------------------------------------*/ 
/* variable defined in scicos.c */
extern CURBLK_struct C2F(curblk);
extern COSIM_struct C2F(cosim);
extern COSERR_struct coserr;    /* declaration of coserr -valable partout- */
/*--------------------------------------------------------------------------*/ 

/*--------------------------------------------------------------------------*/ 
int intsetxproperty(char *fname, unsigned long fname_len)
     /* renvoi le type d'equation get_pointer_xproperty() 
      *	(-1: algebriques, +1 differentielles) */
{
  int un,l1,m1;
  int isrun = C2F(cosim).isrun;

  if (!isrun) {
    Scierror(999,"%s: scicosim is not running.\n",fname);
  }
  else {
    CheckRhs(1,1);
    GetRhsVar(1,"i",&m1,(un=1,&un),&l1);
    set_pointer_xproperty(istk(l1));
    LhsVar(1)=0;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/ 
