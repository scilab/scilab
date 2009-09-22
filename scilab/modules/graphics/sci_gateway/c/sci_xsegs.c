/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
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
/* file: sci_xsegs.c                                                      */
/* desc : interface for xsegs routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "sciCall.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "stack-c.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_xsegs(char *fname,unsigned long fname_len)
{
  int dstyle = -1, *style, colorFlag;
  double * zptr = NULL;
  int mx,nx,lx,my,ny,ly,mz=0,nz=0,lz=0,mc=0,nc=0,lc; 
  const double arsize = 0.0 ; // no arrow here
  sciPointObj * psubwin = NULL ;


  CheckRhs(2,4);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&mx,&nx,&lx);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&my,&ny,&ly);
  CheckSameDims(1,2,mx,nx,my,ny);
  if (my*ny == 0)
	{
		/* Empty segs */
		LhsVar(1)=0;
		C2F(putlhsvar)();
		return 0;
	} 

  if (Rhs == 3)
  {
    GetVarDimension(3,&mz,&nz);
    if( mz*nz == mx*nx)
    {
      GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&mz,&nz,&lz);
      zptr = stk(lz);
    }
    else
    {
      mc=mz; nc=nz; lc=lz;
      if (mc * nc == 1) dstyle = *istk(lc);
      if (mc * nc != 1 && mx*nx / 2 != mc*nc)
      {
        Scierror(999,_("%s: Wrong size for input argument #%d: %d, %d or %d expected.\n"),fname, 3, 1, mx*nx/2, mx*nx);
        return 0;
      }
      GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&mc,&nc,&lc);
      CheckVector(3,mc,nc);
    }
  }

  if (Rhs == 4)
  {
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&mz,&nz,&lz);
    CheckSameDims(1,3,mx,nx,mz,nz);
    zptr = stk(lz);

    GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&mc,&nc,&lc);
    CheckVector(4,mc,nc);

    if (mc * nc != 1 && mx*nx / 2 != mc*nc)
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: %d or %d expected.\n"),fname, 4, 1, mx*nx/2);
      return 0;
    }
  }

  if(mc*nc == 0)
  { /* no color specified, use current color (taken from axes parent) */
    int col;
    psubwin = sciGetCurrentSubWin();
    col   = sciGetForegroundToDisplay(psubwin);
    style = &col;
    colorFlag  = 0;
  }
  else
  {
    style = istk(lc);
    colorFlag = (mc*nc == 1) ? 0 : 1;
  }

  Objsegs (style,colorFlag,mx*nx,stk(lx),stk(ly),zptr,arsize);

  sciDrawObjIfRequired(sciGetCurrentObj ());
    

  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;
} 
/*--------------------------------------------------------------------------*/
