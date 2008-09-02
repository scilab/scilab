/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc
 *    Copyright INRIA 2006
 *    Jean-Philippe Chancelier - jpc@cermics.enpc.fr
 *    Jean-Baptiste Silvy
 *
 *    modified by Bruno Pincon 01/02/2001 for gain in speed and added 
 *    possibilities to set zmin, zmax by the user and also to set the 
 *    first and last color of the colormap (Bruno.Pincon@iecn.u-nancy.fr
 *
for entities handling
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "math_graphics.h" 
#include "Axes.h"

#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DrawObjects.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "localization.h"

/* @TODO : remove this stuff */

/*extern void compute_data_bounds(int cflag,char dataflag,double *x,double *y,int n1,int n2,double *drect);*/
extern void compute_data_bounds2(int cflag,char dataflag,char *logflags,double *x,double *y,int n1,int n2,double *drect);
extern BOOL update_specification_bounds(sciPointObj *psubwin, double *rect,int flag);
extern int re_index_brect(double * brect, double * drect);
extern BOOL strflag2axes_properties(sciPointObj * psubwin, char * strflag);
extern int CreatePrettyGradsFromNax(sciPointObj * psubwin,int * Nax);

void get_frame_in_pixel(integer WIRect[]);

int C2F(fec)(double *x, double *y, double *triangles, double *func, integer *Nnode, integer *Ntr, 
	     char *strflag, char *legend, double *brect, integer *aaint, double *zminmax, 
	     integer *colminmax, integer *colout, BOOL with_mesh, BOOL flagNax, integer lstr1, integer lstr2) ;

/*------------------------------------------------------------
 *  Iso contour with grey level or colors 
 *  for a function defined by finite elements 
 *  ( f is linear on triangles )
 *  we give two versions of the function : 
 *     - a quick version wich only fill triangles according to the average 
 *     value of f on a triangle (no more such version now ?)
 *     - and a slow version but more sexy which use the fact that f is linear
 *     on each triangle.
 *  Nodes (x[no],y[no])
 *  Triangles (Matrix: [ numero, no1,no2,no3,iflag;...]
 *  func[no] : Function value on Nodes.
 *  Nnode : number of nodes 
 *  Ntr   : number of triangles 
 *  strflag,legend,brect,aint : see plot2d
 *  zminmax   : to set (optionnaly) the min and max level
 *  colminmax : to set (optionnaly) the first and last color to use
 *
 *  modified by Bruno Pincon 01/02/2001 for gain in speed and added 
 *  possibilities to set zmin, zmax by the user and also to set the 
 *  first and last color of the colormap (Bruno.Pincon@iecn.u-nancy.fr)
---------------------------------------------------------------*/

int C2F(fec)(double *x, double *y, double *triangles, double *func, integer *Nnode, integer *Ntr, 
	     char *strflag, char *legend, double *brect, integer *aaint, double *zminmax, 
	     integer *colminmax, integer *colout, BOOL with_mesh, BOOL flagNax, integer lstr1, integer lstr2)
{
  integer n1=1;
  
  /* Fec code */
  

  long hdltab[2];
  int cmpt=0;
  sciPointObj * pptabofpointobj;
  sciPointObj * psubwin;
  sciPointObj * pFec;
  sciPointObj * parentCompound;
  double drect[6];

  BOOL bounds_changed = FALSE;
  BOOL axes_properties_changed = FALSE;


  psubwin = sciGetCurrentSubWin();

  checkRedrawing() ;



  /* Force psubwin->is3d to FALSE: we are in 2D mode */
  if (sciGetSurface(psubwin) == (sciPointObj *) NULL)
  {
    pSUBWIN_FEATURE (psubwin)->is3d = FALSE;
    pSUBWIN_FEATURE (psubwin)->project[2]= 0;
  }
  else
  {
    pSUBWIN_FEATURE (psubwin)->theta_kp=pSUBWIN_FEATURE (psubwin)->theta;
    pSUBWIN_FEATURE (psubwin)->alpha_kp=pSUBWIN_FEATURE (psubwin)->alpha;  
  }

  pSUBWIN_FEATURE (psubwin)->alpha  = 0.0;
  pSUBWIN_FEATURE (psubwin)->theta  = 270.0;

  /* Force psubwin->axes.aaint to those given by argument aaint*/
  /*****TO CHANGE F.Leray 10.09.04     for (i=0;i<4;i++) pSUBWIN_FEATURE(psubwin)->axes.aaint[i] = aaint[i]; */

  /* Force "cligrf" clipping */
  sciSetIsClipping (psubwin,0); 

  /* Force  axes_visible property */
  /* pSUBWIN_FEATURE (psubwin)->isaxes  = TRUE;*/

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
      case '2' : case '4' : case '6' : case '8':case '9':
        /* Force psubwin->Srect to the x and y bounds */
        /* compute_data_bounds(0,'g',x,y,n1,*Nnode,drect); */
        compute_data_bounds2(0,'g',pSUBWIN_FEATURE(psubwin)->logflags,x,y,n1,*Nnode,drect);
        break;
    }
    if (!pSUBWIN_FEATURE(psubwin)->FirstPlot &&
      (strflag[1] == '7' || strflag[1] == '8' || strflag[1] == '9')) { /* merge psubwin->Srect and drect */
        drect[0] = Min(pSUBWIN_FEATURE(psubwin)->SRect[0],drect[0]); /*xmin*/
        drect[2] = Min(pSUBWIN_FEATURE(psubwin)->SRect[2],drect[2]); /*ymin*/
        drect[1] = Max(pSUBWIN_FEATURE(psubwin)->SRect[1],drect[1]); /*xmax*/
        drect[3] = Max(pSUBWIN_FEATURE(psubwin)->SRect[3],drect[3]); /*ymax*/
    }
    if (strflag[1] != '0') 
      bounds_changed = update_specification_bounds(psubwin, drect,2);
  } 

  if(pSUBWIN_FEATURE (psubwin)->FirstPlot == TRUE) bounds_changed = TRUE;

  axes_properties_changed = strflag2axes_properties(psubwin, strflag);

  pSUBWIN_FEATURE (psubwin)->FirstPlot = FALSE; /* just after strflag2axes_properties */

  /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and 
  u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */

  pSUBWIN_FEATURE(psubwin)->flagNax = flagNax; /* store new value for flagNax */

  if(pSUBWIN_FEATURE(psubwin)->flagNax == TRUE){
    if(pSUBWIN_FEATURE(psubwin)->logflags[0] == 'n' && pSUBWIN_FEATURE(psubwin)->logflags[1] == 'n')
    {
      BOOL autoTicks[3];
      sciGetAutoTicks(psubwin, autoTicks);
      /* x and y graduations are imposed by Nax */
      sciSetAutoTicks(psubwin, FALSE, FALSE, autoTicks[2]);

      CreatePrettyGradsFromNax(psubwin,aaint);
    }
    else{
      sciprint(_("Warning: Nax does not work with logarithmic scaling.\n"));
    }
  }

  if(bounds_changed || axes_properties_changed )
  {
    forceRedraw(psubwin);
    //sciDrawObj(sciGetCurrentFigure());
  }
  
  /* Construct the object */
  pFec = ConstructFec(psubwin,x,y,triangles,func,
                      *Nnode,*Ntr,zminmax,colminmax,colout, with_mesh); 

  if (pFec == NULL)
  {
    // error in allocation
    sciprint(_("%s: No more memory.\n"), "fec");
    return -1;
  }

  /* Set fec as current */
  sciSetCurrentObj(pFec);

  /* retrieve the created object : fec */
  pptabofpointobj = pFec;
  hdltab[cmpt] = sciGetHandle(pptabofpointobj);   
  cmpt++;   
  
  /** Drawing the Legends **/
  /*
  if ((int)strlen(strflag) >=1  && strflag[0] == '1')
  {
    n1=1; styl[0]=1;styl[1]=0;
    sciSetCurrentObj (ConstructLegend
                      (sciGetCurrentSubWin(),
                       legend,  (int)strlen(legend), n1, styl, &(hdltab[cmpt])); 
    
    
    sciDrawObj(sciGetCurrentObj ()); 
    DrawAxesIfRequired(sciGetCurrentObj ()); 
    hdltab[cmpt]=sciGetHandle(sciGetCurrentObj ()); 
    cmpt++;
  }
  */
  parentCompound = ConstructCompound (hdltab, cmpt);
  sciSetCurrentObj(parentCompound);  /** construct Compound **/

  /* draw every one */
  sciDrawObj(parentCompound);
   
  return(0);
   
}








