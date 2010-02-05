/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - INRIA - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_ticks_utils.c                                                */
/* desc : a set of functions used to return ticks tlist in scilab         */
/*------------------------------------------------------------------------*/

#include <stdio.h>
#include "get_ticks_utils.h"
#include "returnPropertyList.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "Format.h"
#include "SetProperty.h"
/*--------------------------------------------------------------------------------*/
static char ** AllocAndSetUserLabels(char ** u_xlabels, double * u_xgrads, int u_nxgrads, char logflag);
/*--------------------------------------------------------------------------------*/
int buildTListForTicks( const double * locations, char * labels[], int nbTics )
{
  char * variable_tlist[] = {"ticks","locations","labels"};

  returnedList * tList = createReturnedList( 2, variable_tlist ) ;

  if (nbTics == 0)
  {
    // two empty matrices
    addColVectorToReturnedList( tList, NULL, 0 ) ;
    addColVectorToReturnedList( tList, NULL, 0 ) ;
  }
  else
  {
    addColVectorToReturnedList( tList, locations, nbTics ) ;
    addStringColVectorToReturnedList( tList, labels, nbTics ) ;
  }
  
  destroyReturnedList( tList ) ;

  return 0;
}
/*--------------------------------------------------------------------------------*/
double * FreeUserGrads(double * u_xgrads)
{
  FREE(u_xgrads); u_xgrads = NULL;
  return u_xgrads;
}
/*--------------------------------------------------------------------------------*/
int CreatePrettyGradsFromNax(sciPointObj * psubwin,int * Nax)
{
  double xmin = 0, xmax = 0;
  double ymin = 0, ymax = 0;
  int nbtics_x = Nax[1];
  int nbtics_y = Nax[3];
  int nbsubtics_x = Nax[0];
  int nbsubtics_y = Nax[2];
  BOOL autoTicks[3];

  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  
  if(sciGetZooming(psubwin) == TRUE) {
    xmin= ppsubwin->FRect[0]; 
    ymin= ppsubwin->FRect[1]; 
    xmax= ppsubwin->FRect[2];
    ymax= ppsubwin->FRect[3];
  }
  else {
    xmin = ppsubwin->SRect[0];
    ymin = ppsubwin->SRect[2];
    xmax = ppsubwin->SRect[1];
    ymax = ppsubwin->SRect[3];
  }
  
  /* x graduations */
  if(nbtics_x == -1.0)
  {
    // if nbtics_x == -1.0 the user whant autoticks
    sciGetAutoTicks(psubwin, autoTicks);
    sciSetAutoTicks(psubwin, TRUE, autoTicks[1], autoTicks[2]);
  }
  else
  {
    ppsubwin->axes.u_xgrads  = FreeUserGrads (ppsubwin->axes.u_xgrads);
    ppsubwin->axes.u_xlabels = FreeUserLabels(ppsubwin->axes.u_xlabels, &ppsubwin->axes.u_nxgrads);
    
    ppsubwin->axes.u_nxgrads = nbtics_x;
    ppsubwin->axes.u_xgrads = AllocUserGrads(ppsubwin->axes.u_xgrads, nbtics_x);
    
    GraduateWithNax(ppsubwin,&xmin,&xmax,nbtics_x,ppsubwin->axes.u_xgrads);

    ppsubwin->axes.u_xlabels = AllocAndSetUserLabels(ppsubwin->axes.u_xlabels, 
						     ppsubwin->axes.u_xgrads, 
						     ppsubwin->axes.u_nxgrads, 
						     ppsubwin->logflags[0]);
  }

  /* y graduations */
  if(nbtics_y == -1.0)
  {
    // if nbtics_y == -1.0 the user whant autoticks
    sciGetAutoTicks(psubwin, autoTicks);
    sciSetAutoTicks(psubwin, autoTicks[0], TRUE, autoTicks[2]);
  }
  else
  {
    ppsubwin->axes.u_ygrads  = FreeUserGrads (ppsubwin->axes.u_ygrads);
    ppsubwin->axes.u_ylabels = FreeUserLabels(ppsubwin->axes.u_ylabels,
					      &ppsubwin->axes.u_nygrads);
    
    ppsubwin->axes.u_nygrads = nbtics_y;
    ppsubwin->axes.u_ygrads = AllocUserGrads(ppsubwin->axes.u_ygrads, nbtics_y);
    
    GraduateWithNax(ppsubwin,&ymin,&ymax,nbtics_y,ppsubwin->axes.u_ygrads);
    
    ppsubwin->axes.u_ylabels = AllocAndSetUserLabels(ppsubwin->axes.u_ylabels, 
						     ppsubwin->axes.u_ygrads, 
						     ppsubwin->axes.u_nygrads, 
						     ppsubwin->logflags[1]);
  }
    
  /* Subtics storage here */
  ppsubwin->axes.nbsubtics[0] = nbsubtics_x;
  ppsubwin->axes.nbsubtics[1] = nbsubtics_y;

  return 0;
}
/*--------------------------------------------------------------------------------*/
int GraduateWithNax(sciSubWindow * ppsubwin,double *min,double *max,int nbtics, double * grads)
{
  int i;
  double pas;
  
  if(nbtics == 1){
    pas = 0.;
    grads[0] = (*min);
  }
  else{
    pas = (*max - *min) / (nbtics -1);
    
    for(i=0;i<nbtics;i++) 
      grads[i] = (*min) + pas*i;
  }

  return 0;
}
/*--------------------------------------------------------------------------------*/
static char ** AllocAndSetUserLabels(char ** u_xlabels, double * u_xgrads, int u_nxgrads, char logflag)
{
  int i;
  char c_format[5]; 
  int nbtics = u_nxgrads;

  if(u_xgrads == NULL)
    return (char **) NULL;
  
  if(u_xlabels != NULL)
    {
      Scierror(999, _("Impossible: %s must be freed before re-allocating"),"u_xlabels");
      return (char **) NULL;
    }
  

  if((u_xlabels=(char **)MALLOC(u_nxgrads*sizeof(char *)))==NULL){
	  Scierror(999, _("%s: No more memory.\n"), "AllocAndSetUserLabels");
	  return (char **) NULL;
  }

  ChooseGoodFormat(c_format,logflag,u_xgrads,u_nxgrads);
  
  for(i=0;i<nbtics;i++)
    {  
      char foo[100];
      
      sprintf(foo,c_format, u_xgrads[i]);
      
      if((u_xlabels[i]=(char *)MALLOC((strlen(foo)+1)*sizeof(char )))==NULL){
		  Scierror(999, _("%s: No more memory.\n"), "AllocAndSetUserLabels");
		  return (char **) NULL;
      }
      
      strcpy(u_xlabels[i],foo);
    }
  
  return u_xlabels;
}
/*--------------------------------------------------------------------------------*/
char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads)
{
  int i;
  int nbtics = u_nxgrads;

  if(u_nxgrads == 0)
    return (char **) NULL;
  
  if(u_xlabels != NULL)
    {
      Scierror(999, _("Impossible: %s must be freed before re-allocating"),"u_xlabels");
      return (char **) NULL;
    }
  
  if((u_xlabels=(char **)MALLOC(u_nxgrads*sizeof(char *)))==NULL){
	  Scierror(999, _("%s: No more memory.\n"), "AllocAndSetUserLabelsFromMdl");
	  return (char **) NULL;
  }

  
  for(i=0;i<nbtics;i++)
    {  
		if((u_xlabels[i]=(char *)MALLOC((strlen(u_xlabels_MDL[i])+1)*sizeof(char )))==NULL){
			Scierror(999, _("%s: No more memory.\n"), "AllocAndSetUserLabelsFromMdl");
			return (char **) NULL;
      }
      
      strcpy(u_xlabels[i],u_xlabels_MDL[i]);
    }
  
  return u_xlabels;
}
/*--------------------------------------------------------------------------------*/
char ** FreeUserLabels(char ** u_xlabels, int *u_nxgrads)
{
  int i;
  
  if(u_xlabels != NULL){
    for(i=0;i<(*u_nxgrads);i++)
      {FREE(u_xlabels[i]); u_xlabels[i] = (char *) NULL;}
  }
  
  u_xlabels = (char **) NULL;
    
  *u_nxgrads = 0;
  
  return u_xlabels;
}
/*--------------------------------------------------------------------------------*/
double * AllocUserGrads(double * u_xgrads, int nb)
{
  
  if(nb == 0)
    return (double *) NULL;
  
  if(u_xgrads != NULL)
    {
      Scierror(999, _("Impossible: %s must be freed before re-allocating"),"u_xgrads");
      return (double *) NULL;
    }
  
  if((u_xgrads=(double *)MALLOC(nb*sizeof(double)))==NULL){
	  Scierror(999, _("%s: No more memory.\n"),"AllocUserGrads");
	  return (double *) NULL;
  }
    
  return u_xgrads;

}
/*--------------------------------------------------------------------------------*/
int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim)
{
  int i;

  if(u_xgrad_SRC == NULL)
    return 0;

  for(i=0;i<dim;i++)
    u_xgrad_DEST[i] = u_xgrad_SRC[i];
  
  return 0;
}
/*--------------------------------------------------------------------------------*/
