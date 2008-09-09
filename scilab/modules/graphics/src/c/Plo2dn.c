/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 --------------------------------------------------------------------------*/

#include <string.h> /* in case of dbmalloc use */
#include <stdio.h>
#include <math.h>
#include "math_graphics.h"
#include "PloEch.h"

#define spINSIDE_SPARSE
#include "../../sparse/includes/spConfig.h"

#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "Axes.h"
#include "BasicAlgos.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"

#include "MALLOC.h" /* MALLOC */
#include "DrawingBridge.h"
#include "scitokenize.h"
#include "localization.h"

/* @TODO : remove that stuff */
void compute_data_bounds(int cflag, char dataflag,double *x,double *y,integer n1,integer n2,double *drect);
void compute_data_bounds2(int cflag,char dataflag, char * logflags, double *x,double  *y, integer n1,integer n2, double *drect);
BOOL update_specification_bounds(sciPointObj *psubwin, double *rect,int flag);
int re_index_brect(double * brect, double * drect);
extern BOOL strflag2axes_properties(sciPointObj * psubwin, char * strflag);

double * FreeUserGrads(double * u_xgrads);

extern int CreatePrettyGradsFromNax(sciPointObj * psubwin,int * Nax);
extern int GraduateWithNax(sciSubWindow * ppsubwin,double *min,double *max,int nbtics, double * grads);
int ChooseGoodFormat(char * c_format,char logflag, double *_grads,int n_grads);

int plot2dn(integer ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint,BOOL flagNax, integer lstr1,integer lstr2);

static char ** AllocAndSetUserLabels(char ** u_xlabels, double * u_xgrads, int u_nxgrads, char logflag);

/*--------------------------------------------------------------------
 *  plot2dn(ptype,Logflags,x,y,n1,n2,style,strflag,legend,brect,aaint,lstr1,lstr2)
 *  
 *  Draw *n1 curves of *n2 points each
 *
 *  ptype is an integer which gives the polyline drawind mode (0,1,2,3,4)
 *
 *  Logflags is a two character string
 *
 *  (x[i+(*n2)*j] ,y[i+(*n2)*j]) Double values giving the point
 *  position of point i of curve j (i=0,*n2-1 j=0,*n1-1)
 *
 *  style[*n1]-> give the style to use for each curve 
 *     if style is positive --> a mark is used (mark id = style[i])
 *     if style is strictly negative --> a dashed line is used 
 *        (dash id = abs(style[i])
 *     if there's only one curve, style can be of type style[0]=style,
 *     style[1]=pos ( pos in [1,6]) 
 *     pos give the legend position (1 to 6) (this can be iteresting
 *     if you want to superpose curves with different legends by 
 *     calling plot2d more than one time.
 *
 *  strflag[3] is a string
 *  
 *     if strflag[0] == '1' then legends are added 
 *        legend = "leg1@leg2@....@legn"; gives the legend for each curve
 *      else no legend
 *
 *     if strflag[1] == '1' then  the values of brect are used to fix 
 *        the drawing boundaries :  brect[]= <xmin,ymin,xmax,ymax>;
 *      if strflag[1] == '2' then the values  are computed from data
 *      else if strflag[1]=='0' the previous values 
 *                (previous call or defaut values) are used 
 *
 *     if  strflag[2] == '1' ->then an axis is added
 *        the number of intervals 
 *        is specified by the vector aaint[4] of integers 
 *         <aaint[0],aaint[1]> specifies the x-axis number of  points 
 *         <aaint[2],aaint[3]> same for y-axis
 *     if  strflag[2] == '2' -> no axis, only a box around the curves
 *     else no box and no axis 

 * lstr* : unused ( but used by Fortran ) 
 *--------------------------------------------------------------------------*/
  
int plot2dn(integer ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint,BOOL flagNax, integer lstr1,integer lstr2)
{
  int closeflag = 0;
  int jj = 0;
  long long *tabofhandles = NULL ;
  sciPointObj  *psubwin = NULL ;
  sciPointObj * curFigure = NULL;
  long hdl;
  long *hdltab = NULL ;
  int cmpt=0;
  int with_leg;
  double drect[6];
  char dataflag;
  sciSubWindow * ppsubwin = NULL;
  BOOL bounds_changed = FALSE;
  BOOL axes_properties_changed = FALSE;

  startGraphicDataWriting();
  curFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();
  ppsubwin = pSUBWIN_FEATURE(psubwin);
  endGraphicDataWriting();

  startFigureDataWriting(curFigure);

  /* check if the auto_clear property is on and then erase everything */
  checkRedrawing() ;
  
  if (sciGetSurface(psubwin) == (sciPointObj *) NULL) /* F.Leray 18.05.04 */
    {
      ppsubwin->is3d = FALSE;
      ppsubwin->project[2]= 0;
    }
  else
    {
      ppsubwin->theta_kp=ppsubwin->theta;
      ppsubwin->alpha_kp=ppsubwin->alpha;
    }
  
  ppsubwin->alpha  = 0.0;
  ppsubwin->theta  = 270.0;

  /* Force psubwin->logflags to those given by argument*/
  if ( ppsubwin->FirstPlot )
  {
    char newLogFlags[3];
    sciGetLogFlags(psubwin, newLogFlags);
    newLogFlags[0] = logflags[1];
    newLogFlags[1] = logflags[2];
    sciSetLogFlags(psubwin, newLogFlags);
  }

  /* Force "cligrf" clipping */
  sciSetIsClipping (psubwin,0); 

  if (sciGetGraphicMode (psubwin)->autoscaling) {
    /* compute and merge new specified bounds with psubwin->Srect */
    switch (strflag[1])  {
    case '0': 
      /* do not change psubwin->Srect */
      break;
    case '1' : case '3' : case '5' : case '7':
      /* Force psubwin->Srect=brect */
      re_index_brect(brect, drect);
      break;
    case '2' : case '4' : case '6' : case '8': case '9':
      /* Force psubwin->Srect to the x and y bounds */
      if ( (int)strlen(logflags) < 1) dataflag='g' ; else dataflag=logflags[0];
      compute_data_bounds2(0,dataflag,ppsubwin->logflags,x,y,*n1,*n2,drect);
      break;
    }
    if (!pSUBWIN_FEATURE(psubwin)->FirstPlot && 
	(strflag[1] == '5' || strflag[1] == '7' || strflag[1] == '8' || strflag[1] == '9')) { /* merge psubwin->Srect and drect */
      
      drect[0] = Min(ppsubwin->SRect[0],drect[0]); /*xmin*/
      drect[2] = Min(ppsubwin->SRect[2],drect[2]); /*ymin*/
      drect[1] = Max(ppsubwin->SRect[1],drect[1]); /*xmax*/
      drect[3] = Max(ppsubwin->SRect[3],drect[3]); /*ymax*/
      
    }
    if (strflag[1] != '0')
      bounds_changed = update_specification_bounds(psubwin, drect,2);
  } 
  
  if(ppsubwin->FirstPlot == TRUE) bounds_changed = TRUE;
  
  axes_properties_changed = strflag2axes_properties(psubwin, strflag);
     
  ppsubwin->FirstPlot = FALSE; /* just after strflag2axes_properties */
   
  with_leg= (strflag[0] == '1');
  ppsubwin->with_leg = with_leg;

  /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and 
     u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */
  
  ppsubwin->flagNax = flagNax; /* store new value for flagNax */

  if(ppsubwin->flagNax == TRUE){
    if(ppsubwin->logflags[0] == 'n' && ppsubwin->logflags[1] == 'n')
      {
        BOOL autoTicks[3];
        sciGetAutoTicks(psubwin, autoTicks);
        /* x and y graduations are imposed by Nax */
        sciSetAutoTicks(psubwin, FALSE, FALSE, autoTicks[2]);
	
	CreatePrettyGradsFromNax(psubwin,aaint);
      }
    else
    {
      sciprint(_("Warning : Nax does not work with logarithmic scaling.\n"));
    }
  }
  endFigureDataWriting(curFigure);
  
  /*if(bounds_changed == TRUE || axes_properties_changed == TRUE)
  {
    
    sciDrawObj(sciGetCurrentFigure());
  }*/
  
  /*---- Drawing the curves and the legends ----*/
  if ( *n1 != 0 ) {
	  if ((hdltab = MALLOC ((*n1+1) * sizeof (long))) == NULL) {
		  sciprint(_("%s: No more memory.\n"),"plot2d");
		  return 0;
    }
    if (with_leg) {
      /* tabofhandles allocated for legends */
      if ((tabofhandles = MALLOC((*n1)*sizeof(long long))) == NULL) {
        sciprint(_("%s: No more memory.\n"),"plot2d");
        FREE(hdltab);
        return 0;
      }
    }

    startFigureDataWriting(curFigure);
    for (jj = 0;jj < *n1; jj++) {/*A.Djalel 3D axes*/
      sciPointObj * pobj = NULL;
      if (style[jj] > 0) { 
	BOOL isline = TRUE;
	if (ptype==3)
        {
          isline=FALSE;
        }
	pobj = ConstructPolyline(sciGetCurrentSubWin(),&(x[jj*(*n2)]),
	         	         &(y[jj*(*n2)]),PD0,closeflag,*n2,ptype,
			         &style[jj],NULL,NULL,NULL,NULL, 
			         isline,FALSE,FALSE,FALSE);
      }
      else {
	int minusstyle = -style[jj];
	pobj = ConstructPolyline(sciGetCurrentSubWin(),&(x[jj*(*n2)]),
			         &(y[jj*(*n2)]),PD0,closeflag,*n2,ptype,
			         NULL,NULL,&minusstyle,NULL,NULL,FALSE,FALSE,TRUE,FALSE);
      }
      if (pobj == NULL)
      {
        // skip
        sciprint(_("%s: No more memory.\n"), "plot2d");
      }
      else
      {
        
        sciSetCurrentObj(pobj);
      
        /*sciDrawObjIfRequired(pobj);*/
        hdl=sciGetHandle(pobj);
        if (with_leg)
        {
          tabofhandles[cmpt] = hdl;
        }

        hdltab[cmpt]=hdl;
        cmpt++;
      }
      
    }
    endFigureDataWriting(curFigure);
    /*DrawAxesIfRequired(sciGetCurrentObj ());*/ /* force axes redrawing once is sufficient (F.Leray 10.01.05) */
    forceRedraw(psubwin);
    
    /*---- Drawing the Legends ----*/
    startFigureDataWriting(curFigure);

    if (with_leg) {
      char ** Str;
      int nleg;
      sciPointObj * Leg;
      if (scitokenize(legend, &Str, &nleg)) {
	FREE(tabofhandles);
	FREE(hdltab);
	sciprint(_("%s: No more memory.\n"),"plot2d");
        endFigureDataWriting(curFigure);
	return 0;
      }

      /* if (nleg < cmpt) {
	FREE(tabofhandles);
        for (jj = 0; jj < nleg; jj++) { FREE(Str[jj]); }
	FREE(Str);
	sciprint(_("%s: Invalid legend ignored.\n"),"plot2d");
      }
      else {*/
      Leg = ConstructLegend(sciGetCurrentSubWin(),Str,tabofhandles,Min(nleg,cmpt));
	if (Leg != NULL)
	  {
	    pLEGEND_FEATURE(Leg)->place = SCI_LEGEND_LOWER_CAPTION;
	    sciSetIsFilled (Leg, FALSE);
	    sciSetIsLine (Leg, FALSE);
	    sciSetCurrentObj (Leg); 
	  }

      
	for (jj = 0; jj < nleg; jj++) { FREE(Str[jj]); }
	FREE(Str);
	FREE(tabofhandles);
	/* }*/
    }

    /*---- construct Compound ----*/
    if (cmpt > 0)
    {
      sciSetCurrentObj(ConstructCompound (hdltab, cmpt));
    }
    FREE(hdltab);
    endFigureDataWriting(curFigure);
    
  }
  
  sciDrawObj(curFigure);
  return(0);
}




/* Given two set of coordinates x and y this routine computes the corresponding 
 *  data bounds rectangle drect=[xmin,ymin,xmax,ymax] taking into account the logflag
 *  -> means we have to find among the data the min > 0.
 */
void compute_data_bounds2(int cflag,char dataflag, char * logflags, double *x,double  *y, integer n1,integer n2, double *drect)
{
  int size_x,size_y;
  double xd[2];
  double *x1;
  switch ( dataflag ) {
  case 'e' : 
    xd[0] = 1.0; xd[1] = (double)n2;
    x1 = xd;size_x = (n2 != 0) ? 2 : 0 ;
    break; 
  case 'o' : 
    x1 = x;size_x = n2;
    break;
  case 'g' : 
  default  : 
    x1 = x;size_x = (cflag == 1) ? n1 : (n1*n2) ;
    break; 
  }

  if (size_x != 0) {
    if(logflags[0] != 'l'){
      drect[0] =  Mini(x1, size_x); 
      drect[1] =  Maxi(x1,size_x); 
    }
    else { /* log. case */
      drect[0] =  sciFindStPosMin(x1,size_x); 
      drect[1] =  Maxi(x1,size_x); 
    }
    
  }
  else {
    if(logflags[0] != 'l'){
      drect[0] = 0.0;
      drect[1] = 10.0;
    }
    else{/* log. case */
      drect[0] = 1.0;
      drect[1] = 10.0;
    }
  }

  size_y = (cflag == 1) ? n2 : (n1*n2) ;
  if (size_y != 0) {
    if(logflags[1] != 'l'){
      drect[2] =  Mini(y, size_y); 
      drect[3] =  Maxi(y,size_y); 
    }
    else{/* log. case */
      drect[2] =  sciFindStPosMin(y,size_y); 
      drect[3] =  Maxi(y,size_y); 
    }

  }
  else {
    if(logflags[1] != 'l'){
      drect[2] = 0.0;
      drect[3] = 10.0;
    }
    else{/* log. case */
      drect[2] = 1.0;
      drect[3] = 10.0;
    }
  }
  /* back to default values for  x=[] and y = [] */
  if ( drect[2] == LARGEST_REAL ) { drect[2] = 0.0; drect[3] = 10.0 ;} 
  if ( drect[0] == LARGEST_REAL ) { drect[0] = 0.0; drect[1] = 10.0 ;} 

}




/* Given two set of coordinates x and y this routine computes the corresponding 
 *  data bounds rectangle drect=[xmin,ymin,xmax,ymax] 
 */
void compute_data_bounds(int cflag, char dataflag,double *x,double *y,integer n1,integer n2,double *drect)
{
  int size_x,size_y;
  double xd[2];
  double *x1;
  switch ( dataflag ) {
  case 'e' : 
    xd[0] = 1.0; 
	xd[1] = (double)n2;
    x1 = xd;
	size_x = (n2 != 0) ? 2 : 0 ;
    break; 
  case 'o' : 
    x1 = x;
	size_x = n2;
    break;
  case 'g' : 
  default  : 
    x1 = x;
	size_x = (cflag == 1) ? n1 : (n1*n2) ;
    break; 
  }

  if (size_x != 0) {
    drect[0] =  Mini(x1, size_x); 
    drect[1] =  Maxi(x1,size_x); 
  }
  else {
    drect[0] = 0.0;
    drect[1] = 10.0;
  }

  size_y = (cflag == 1) ? n2 : (n1*n2) ;
  if (size_y != 0) {
    drect[2] =  Mini(y, size_y); 
    drect[3] =  Maxi(y,size_y); 
  }
  else {
    drect[2] = 0.0;
    drect[3] = 10.0;
  }
  /* back to default values for  x=[] and y = [] */
  if ( drect[2] == LARGEST_REAL ) { 
	  drect[2] = 0.0; drect[3] = 10.0 ;
  } 
  if ( drect[0] == LARGEST_REAL ) {
	  drect[0] = 0.0; drect[1] = 10.0 ;
  }

}

BOOL update_specification_bounds(sciPointObj  *psubwin,double rect[6],int flag)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  BOOL haschanged = FALSE;
  
  ppsubwin->SRect[0] = rect[0];
  ppsubwin->SRect[1] = rect[1];
  ppsubwin->SRect[2] = rect[2];
  ppsubwin->SRect[3] = rect[3];
  if (flag==3) {
    ppsubwin->SRect[4] = rect[4];
    ppsubwin->SRect[5] = rect[5];
  }
  
  if(flag != 3)
    haschanged = sci_update_frame_bounds_2d(psubwin);
  else
    haschanged = sci_update_frame_bounds_3d(psubwin);
  
  
  return haschanged;
}



static char ** AllocAndSetUserLabels(char ** u_xlabels, double * u_xgrads, int u_nxgrads, char logflag)
{
  int i;
  char c_format[5]; 
  int nbtics = u_nxgrads;

  if(u_xgrads == NULL)
    return (char **) NULL;
  
  if(u_xlabels != NULL)
    {
      sciprint(_("Impossible: %s must be freed before re-allocating"),"u_xlabels");
      return (char **) NULL;
    }
  

  if((u_xlabels=(char **)MALLOC(u_nxgrads*sizeof(char *)))==NULL){
	  sciprint(_("%s: No more memory.\n"), "AllocAndSetUserLabels");
	  return (char **) NULL;
  }

  ChooseGoodFormat(c_format,logflag,u_xgrads,u_nxgrads);
  
  for(i=0;i<nbtics;i++)
    {  
      char foo[100];
      
      sprintf(foo,c_format, u_xgrads[i]);
      
      if((u_xlabels[i]=(char *)MALLOC((strlen(foo)+1)*sizeof(char )))==NULL){
		  sciprint(_("%s: No more memory.\n"), "AllocAndSetUserLabels");
		  return (char **) NULL;
      }
      
      strcpy(u_xlabels[i],foo);
    }
  
  return u_xlabels;
}



char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads)
{
  int i;
  int nbtics = u_nxgrads;

  if(u_nxgrads == 0)
    return (char **) NULL;
  
  if(u_xlabels != NULL)
    {
      sciprint(_("Impossible: %s must be freed before re-allocating"),"u_xlabels");
      return (char **) NULL;
    }
  
  if((u_xlabels=(char **)MALLOC(u_nxgrads*sizeof(char *)))==NULL){
	  sciprint(_("%s: No more memory.\n"), "AllocAndSetUserLabelsFromMdl");
	  return (char **) NULL;
  }

  
  for(i=0;i<nbtics;i++)
    {  
		if((u_xlabels[i]=(char *)MALLOC((strlen(u_xlabels_MDL[i])+1)*sizeof(char )))==NULL){
			sciprint(_("%s: No more memory.\n"), "AllocAndSetUserLabelsFromMdl");
			return (char **) NULL;
      }
      
      strcpy(u_xlabels[i],u_xlabels_MDL[i]);
    }
  
  return u_xlabels;
}




/* F.Leray */
/* brect must have the same format as drect i.e.: [xmin,xmax,ymin,ymax] */
/* brect = INPUT ; drect = OUTPUT (warning drect is dim 6) */
int re_index_brect(double * brect, double * drect)
{
  drect[0] = brect[0];
  drect[1] = brect[2];
  drect[2] = brect[1];
  drect[3] = brect[3];
/*  drect[4] = brect[4];
    drect[5] = brect[5];*/
  
  return 0;
}
/* F.Leray 07.05.04 */
/* Dispatch info contained in strflag to all the flag available in
   sciSubwin object (tight_limits, isoview, isaxes...) */
BOOL strflag2axes_properties(sciPointObj * psubwin, char * strflag)
{
  BOOL haschanged = FALSE;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);

  /* F.Leray 07.05.04 */
  /* strflag[1] Isoview & tight_limits flags management*/
  switch (strflag[1])  {
  case '0': case '9':
    /* no changes */
    break;
  case '1' : case '2' : case '7' : case '8' :
    if(ppsubwin->tight_limits != TRUE){
      ppsubwin->tight_limits = TRUE;
      haschanged = TRUE;
    }
    /*pSUBWIN_FEATURE (psubwin)->isoview      = FALSE; */
    break;
  case '3' : case '4' :
    /*pSUBWIN_FEATURE (psubwin)->tight_limits = TRUE;*/
    if(ppsubwin->isoview != TRUE){
      ppsubwin->isoview = TRUE;
      haschanged = TRUE;
    }
    break;
  case '5' : case '6' :
    if(ppsubwin->tight_limits != FALSE){
      ppsubwin->tight_limits = FALSE; /* pretty axes */
      haschanged = TRUE;
    }
    /*pSUBWIN_FEATURE (psubwin)->isoview      = FALSE;*/
    break;
  }
      
  /* F.Leray 07.05.04 */
  /* strflag[2] */
  switch (strflag[2])  {
  case '0': 
    if(ppsubwin->FirstPlot == TRUE){
      /*       ppsubwin->isaxes = FALSE; */
      ppsubwin->axes.axes_visible[0] = FALSE;
      ppsubwin->axes.axes_visible[1] = FALSE;
      ppsubwin->axes.axes_visible[2] = FALSE; /* also trigger z axis */
      ppsubwin->axes.rect = BT_OFF;
      haschanged = TRUE;
    }
    /*else no changes : the isaxes properties is driven by the previous plot */
    break;
  case '1' : 
 /*    ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE ||
       ppsubwin->axes.ydir != 'l'){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.ydir ='l';
      ppsubwin->axes.rect = BT_ON;
      haschanged = TRUE;
    }
    break;
  case '2' : 
  /*   ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE){
      ppsubwin->axes.axes_visible[0] = FALSE;
      ppsubwin->axes.axes_visible[1] = FALSE;
      ppsubwin->axes.axes_visible[2] = FALSE; /* also trigger z axis */
      ppsubwin->axes.rect = BT_ON;
      haschanged = TRUE;
    }
    break;
  case '3' : 
    /*    ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE ||
       ppsubwin->axes.ydir != 'r'){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.ydir ='r';
      haschanged = TRUE;
    }
    break;
  case '4' :
    /*     ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.rect = BT_ON;
      /* Case not implemented yet : axes are drawn centred in the middle of the frame box. */
      haschanged = TRUE;
    }
    break;
  case '5' :
 /*    ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE ||
       ppsubwin->axes.xdir != 'c' ||
       ppsubwin->axes.ydir != 'c'){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      ppsubwin->axes.xdir ='c';
      ppsubwin->axes.ydir ='c';
      ppsubwin->axes.rect = BT_ON;
      haschanged = TRUE;
    }
    break;
  case '9' :
    /*     ppsubwin->isaxes = TRUE; */
    if(ppsubwin->axes.axes_visible[0] != TRUE ||
       ppsubwin->axes.axes_visible[1] != TRUE ||
       ppsubwin->axes.axes_visible[2] != TRUE){
      ppsubwin->axes.axes_visible[0] = TRUE;
      ppsubwin->axes.axes_visible[1] = TRUE;
      ppsubwin->axes.axes_visible[2] = TRUE; /* also trigger z axis */
      haschanged = TRUE;
    }
  }
  return haschanged;
}


int CreatePrettyGradsFromNax(sciPointObj * psubwin,int * Nax)
{
  double xmin = 0, xmax = 0;
  double ymin = 0, ymax = 0;
  int nbtics_x = Nax[1];
  int nbtics_y = Nax[3];
  int nbsubtics_x = Nax[0];
  int nbsubtics_y = Nax[2];

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
  ppsubwin->axes.u_xgrads  = FreeUserGrads (ppsubwin->axes.u_xgrads);
  ppsubwin->axes.u_xlabels = FreeUserLabels(ppsubwin->axes.u_xlabels,
					    &ppsubwin->axes.u_nxgrads);
  
  ppsubwin->axes.u_nxgrads = nbtics_x;
  ppsubwin->axes.u_xgrads = AllocUserGrads(ppsubwin->axes.u_xgrads, nbtics_x);
  
/*   GraduateWithNax(ppsubwin,xmin,xmax,xoutmin,xoutmax,nbtics_x,nbsubtics_x,  */
/* 		  ppsubwin->axes.u_xgrads, ppsubwin->axes.u_nxgrads); */
  
  GraduateWithNax(ppsubwin,&xmin,&xmax,nbtics_x,ppsubwin->axes.u_xgrads);

  ppsubwin->axes.u_xlabels = AllocAndSetUserLabels(ppsubwin->axes.u_xlabels, 
						   ppsubwin->axes.u_xgrads, 
						   ppsubwin->axes.u_nxgrads, 
						   ppsubwin->logflags[0]);
  
  /* y graduations */
  ppsubwin->axes.u_ygrads  = FreeUserGrads (ppsubwin->axes.u_ygrads);
  ppsubwin->axes.u_ylabels = FreeUserLabels(ppsubwin->axes.u_ylabels,
					    &ppsubwin->axes.u_nygrads);
  
  ppsubwin->axes.u_nygrads = nbtics_y;
  ppsubwin->axes.u_ygrads = AllocUserGrads(ppsubwin->axes.u_ygrads, nbtics_y);
  
  /*   GraduateWithNax(ppsubwin,ymin,ymax,youtmin,youtmax,nbtics_y,nbsubtics_y,  */
/* 		  ppsubwin->axes.u_ygrads, ppsubwin->axes.u_nygrads); */
  
  GraduateWithNax(ppsubwin,&ymin,&ymax,nbtics_y,ppsubwin->axes.u_ygrads);
  
  ppsubwin->axes.u_ylabels = AllocAndSetUserLabels(ppsubwin->axes.u_ylabels, 
						   ppsubwin->axes.u_ygrads, 
						   ppsubwin->axes.u_nygrads, 
						   ppsubwin->logflags[1]);
    
  /* Subtics storage here */
  ppsubwin->axes.nbsubtics[0] = nbsubtics_x;
  ppsubwin->axes.nbsubtics[1] = nbsubtics_y;

  return 0;
}

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


double * FreeUserGrads(double * u_xgrads)
{
  FREE(u_xgrads); u_xgrads = NULL;
  return u_xgrads;
}

double * AllocUserGrads(double * u_xgrads, int nb)
{
  
  if(nb == 0)
    return (double *) NULL;
  
  if(u_xgrads != NULL)
    {
      sciprint(_("Impossible: %s must be freed before re-allocating"),"u_xgrads");
      return (double *) NULL;
    }
  
  if((u_xgrads=(double *)MALLOC(nb*sizeof(double)))==NULL){
	  sciprint(_("%s: No more memory.\n"),"AllocUserGrads");
	  return (double *) NULL;
  }
    
  return u_xgrads;

}

int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim)
{
  int i;

  if(u_xgrad_SRC == NULL)
    return 0;

  for(i=0;i<dim;i++)
    u_xgrad_DEST[i] = u_xgrad_SRC[i];
  
  return 0;
}

