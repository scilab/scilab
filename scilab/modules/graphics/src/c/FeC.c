/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2001 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2005-2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2001 -  Bruno Pincon (for gain in speed and added 
 *    possibilities to set zmin, zmax by the user and also to set the 
 *    first and last color of the colormap)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 * for entities handling
 */

#include "machine.h"
#include "math_graphics.h" 
#include "Axes.h"

#include "SetProperty.h"
#include "BuildObjects.h"
#include "sciprint.h"
#include "Scierror.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "localization.h"
#include "PloEch.h"
#include "Plot2d.h"
#include "get_ticks_utils.h"
#include "HandleManagement.h"


/**
 * Before Scilab 5.1, default colout was [-1, -1].
 * However, to be more coherent with other fec object properties
 * (which were interfaced from Scilab in version 5.1),
 * the default value is now [0,0].
 * However, to keep backward compatibility, we made a patch for
 * the fec function.
 * In other word, this function converts colout from fec function
 * colout argument to fec handle outside_colors properties.
 */
static void coloutPatch(int colout[2]);

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

int C2F(fec)(double *x, double *y, double *triangles, double *func, int *Nnode, int *Ntr, 
	     char *strflag, char *legend, double *brect, int *aaint, double *zminmax, 
	     int *colminmax, int *colout, BOOL with_mesh, BOOL flagNax, int lstr1, int lstr2)
{
  int n1=1;
  
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
  }
  
  /* Construct the object */
	/* Patch on colout */
	/* For coherence with other properties, default colout is [0, 0] for fec handles instead of  */
	/* [-1,-1] */
	coloutPatch(colout);
  pFec = ConstructFec(psubwin,x,y,triangles,func,
                      *Nnode,*Ntr,zminmax,colminmax,colout, with_mesh); 

  if (pFec == NULL)
  {
    // error in allocation
    Scierror(999, _("%s: No more memory.\n"), "fec");
    return -1;
  }

  /* Set fec as current */
  sciSetCurrentObj(pFec);

  /* retrieve the created object : fec */
  pptabofpointobj = pFec;
  hdltab[cmpt] = sciGetHandle(pptabofpointobj);   
  cmpt++;   
  
  parentCompound = ConstructCompound (hdltab, cmpt);
  sciSetCurrentObj(parentCompound);  /** construct Compound **/

  /* draw every one */
  sciDrawObj(parentCompound);
   
  return(0);
   
}
/*--------------------------------------------------------------------------*/
static void coloutPatch(int colout[2])
{
	if (colout[0] < 0)
	{
		/* default mode */
		colout[0] = 0;
	}
	else if (colout[0] == 0)
	{
		/* transparent facet */
		colout[0] = -1;
	}

	if (colout[1] < 0)
	{
		/* default mode */
		colout[1] = 0;
	}
	else if (colout[1] == 0)
	{
		/* transparent facet */
		colout[1] = -1;
	}

}
/*--------------------------------------------------------------------------*/
