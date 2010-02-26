/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_log_flags_property.c                                         */
/* desc : function to modify in Scilab the log_flags field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"

/*--------------------------------------------------------------------------*/
char ** ReBuildUserTicks( char old_logflag, char new_logflag, double * u_xgrads, int *u_nxgrads, char ** u_xlabels);
/*--------------------------------------------------------------------------*/
char ** CaseLogflagN2L(int * u_nxgrads, double *u_xgrads, char ** u_xlabels);
/*--------------------------------------------------------------------------*/
/* Remove negative graduations when switching from N (linear) to L (logarithmic) scale */
char ** CaseLogflagN2L(int * u_nxgrads, double *u_xgrads, char ** u_xlabels)
{
  int nbtics = *u_nxgrads;
  int i;
  char ** ticklabel = (char **) NULL;
  int cmpteur = 0, cmpteur2 = 0, offset = 0;


  for(i=0;i<nbtics;i++) 
  {
    if(u_xgrads[i]<=0){
      sciprint("Warning: graduation number %d is ignored : when switching to logarithmic scale, we must have strictly positive graduations!\n",i);
    }
    else
    {
      u_xgrads[cmpteur] = log10(u_xgrads[i]);
      cmpteur++;
    }
  }

  if(cmpteur != nbtics)
  {
    if((ticklabel=(char **)MALLOC(cmpteur*sizeof(char *)))==NULL){
      Scierror(999, _("%s: No more memory.\n"),"CaseLogflagN");
			return NULL;
    }

    cmpteur2 = 0;
    offset = nbtics - cmpteur;
    for(i=0;i<cmpteur;i++){
      if((ticklabel[cmpteur2]=(char *)MALLOC((strlen(u_xlabels[i+offset])+1)*sizeof(char )))==NULL){
				Scierror(999, _("%s: No more memory.\n"),"CaseLogflagN");
      }
      strcpy(ticklabel[cmpteur2],u_xlabels[i+offset]);
      cmpteur2++;
    }

	freeArrayOfString(u_xlabels, nbtics);
    u_xlabels = ticklabel;
  }

  *u_nxgrads = cmpteur;
  cmpteur = 0;
  cmpteur2 = 0;

  return u_xlabels;
}
/*--------------------------------------------------------------------------*/
/* Called by a.log_flags='nn','ln','nl', or 'll'*/
/* For the moment, z has no logflag F.Leray 05.10.04 */
char ** ReBuildUserTicks( char old_logflag, char new_logflag, double * u_xgrads, int *u_nxgrads, char ** u_xlabels)
{

  if(old_logflag==new_logflag) { return u_xlabels; } /* nothing to do l->l or n->n */

  if(u_xgrads!=NULL)
  {
    if(old_logflag=='n' && new_logflag=='l') /* n->l */ /* 10-> 1, 100->2 ...*/
    {

      u_xlabels=CaseLogflagN2L(u_nxgrads,u_xgrads,u_xlabels);

    }
    else if(old_logflag=='l' && new_logflag=='n')
    {
      int nbtics = *u_nxgrads;
      int i;

      for(i=0;i<nbtics;i++) u_xgrads[i] = exp10(u_xgrads[i]);

    }
  }

  return  u_xlabels;
}
/*------------------------------------------------------------------------*/
int set_log_flags_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  char * flags;
  sciSubWindow * ppSubWin = NULL ;
  char curLogFlags[4] = "nnn";

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "log_flags");
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) != SCI_SUBWIN)
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"log_flags") ;
    return SET_PROPERTY_ERROR ;
  }
  
  if ( nbRow * nbCol != 2 && nbRow * nbCol != 3 )
  {
    Scierror(999, _("Wrong size for '%s' property: Must be %s or %s.\n"), "log_flags", "2", "3");
    return SET_PROPERTY_ERROR ;
  }

  flags = getStringFromStack(stackPointer);

  /* flags must be 'n' or 'l' */
  if (   (flags[0] != 'n' && flags[0] != 'l')
      || (flags[1] != 'n' && flags[1] != 'l'))
  {
    Scierror(999, _("%s: Wrong value for argument: '%s' or '%s' expected.\n"),"flags","n","l");
    return SET_PROPERTY_ERROR ;
  }

  ppSubWin = pSUBWIN_FEATURE (pobj) ;

  // get a copy of current log flags
  sciGetLogFlags(pobj, curLogFlags);


  /* X axes */
  if( ( ppSubWin->SRect[0] <= 0. || ppSubWin->SRect[1] <= 0.) && flags[0] == 'l' )
  {
    Scierror(999, _("Error: data_bounds on %s axis must be strictly positive to switch to logarithmic mode.\n"),"x");
    return SET_PROPERTY_ERROR ;
  }
  ppSubWin->axes.u_xlabels = ReBuildUserTicks( curLogFlags[0], flags[0],
                                               ppSubWin->axes.u_xgrads, 
                                               &ppSubWin->axes.u_nxgrads, 
                                               ppSubWin->axes.u_xlabels   );


  curLogFlags[0] = flags[0];

  /* Y axes */
  if( ( ppSubWin->SRect[2] <= 0. || ppSubWin->SRect[3] <= 0. ) && flags[1] == 'l' )
  { 
      Scierror(999, _("Error: data_bounds on %s axis must be strictly positive to switch to logarithmic mode.\n"),"y");
      return SET_PROPERTY_ERROR ;
  }
  ppSubWin->axes.u_ylabels = ReBuildUserTicks( curLogFlags[1], flags[1],  
                                               ppSubWin->axes.u_ygrads, 
                                               &ppSubWin->axes.u_nygrads, 
                                               ppSubWin->axes.u_ylabels  ) ;

  curLogFlags[1] = flags[1];


  if ( nbRow * nbCol == 3 )
  {
    if ( flags[2] != 'n' && flags[2] != 'l' )
    {
      Scierror(999, "flags must be 'n' or 'l'.\n") ;
      return SET_PROPERTY_ERROR ;
    }

    if ( ( ppSubWin->SRect[4] <= 0. || ppSubWin->SRect[5] <= 0. ) && flags[2] == 'l' )
    {
      Scierror(999, _("Error: data_bounds on %s axis must be strictly positive to switch to logarithmic mode.\n"),"z");
      return SET_PROPERTY_ERROR ;
    }

    ppSubWin->axes.u_zlabels = ReBuildUserTicks( curLogFlags[2], flags[2],  
                                                 ppSubWin->axes.u_zgrads, 
                                                 &ppSubWin->axes.u_nzgrads, 
                                                 ppSubWin->axes.u_zlabels) ;
    curLogFlags[2] = flags[2];

  }

  sciSetLogFlags(pobj, curLogFlags);


  return SET_PROPERTY_SUCCEED ;


}
/*------------------------------------------------------------------------*/
