/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA - Scilab 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#ifdef _MSC_VER
	#include <Windows.h>
	#pragma comment(lib, "winmm.lib")
#endif
#include "gw_sound.h"
#include "stack-c.h"
#include "cluni0.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static char filename[PATH_MAX];
static int out_n;
static long int lout;
/*--------------------------------------------------------------------------*/
int C2F(playsound)(char *fname,char *command,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
/* SCILAB function : PlaySound */
/*--------------------------------------------------------------------------*/
int sci_Playsound (char *fname,unsigned long fname_len)
{
  char *command=NULL;
  int m1,n1,l1,un=1,rep,m2,n2,l2;
  CheckRhs(1,2);
  CheckLhs(0,1);
  /*  checking variable file */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  if ( Rhs == 2 )
  {
      GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
      command = cstk(l2);
  }
  /*** first call to get the size **/
  lout=PATH_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  rep = C2F(playsound)(filename,command,(int)strlen(filename));
  if ( Lhs == 1 )
  {
      CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&l2);
      *stk(l2)= rep;
      LhsVar(1)=Rhs+2;
  }
  else
  {
    if ( rep == -1 )
	{
		Scierror(999,_("%s: An error occurred: %s\n"),fname,_("Cannot play file.") );
	}
    LhsVar(1)=0;
  }
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(playsound)(char *fname,char *command,unsigned long fname_len)
{

#ifdef _MSC_VER
  /* Stop Playing*/
  PlaySound(NULL,NULL,SND_PURGE);
  /* Play Wav file	*/
  PlaySound(filename,NULL,SND_ASYNC|SND_FILENAME);
  return 0;
#else
  /* linux : a player should be detected by configure ?
   */
  /* !!!! This code MUST be rewrited !!! It is not at all the way of playing
	 sound with Linux */
  char system_cmd[PATH_MAX+10];
  int rep ;
  sprintf(system_cmd,"%s  %s > /dev/null 2>&1",(command == NULL) ? "play": command, filename);
  rep = system(system_cmd);
  return rep;
#endif
}
/*--------------------------------------------------------------------------*/
