/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan Cornet
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/*--------------------------------------------------------------------------*/
#include "nographics.h"
#include "WindowList.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int gw_graphics(void)
{
	Scierror(999,_("Scilab Graphics module not installed.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
void loadGraphicModule( void )
{

}
/*--------------------------------------------------------------------------*/
BOOL TerminateGraphics(void)
{
	return TRUE;
}
/*--------------------------------------------------------------------------*/
int sciSetViewport( sciPointObj * pObj, int xSize, int ySize )
{
	return 0;
}
/*--------------------------------------------------------------------------*/
void sciGetViewport( sciPointObj * pObj, int * xSize, int * ySize )
{

}
/*--------------------------------------------------------------------------*/
double Maxi(double *vect,int n)
{
	return 0.0;
}
/*--------------------------------------------------------------------------*/
double Mini(double *vect, int n)
{
	return 0.0;
}
/*--------------------------------------------------------------------------*/
BOOL sciHasFigures( void )
{
  return FALSE;
}
/*--------------------------------------------------------------------------*/
char **getDictionaryGetProperties(int *sizearray)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **getDictionarySetProperties(int *sizearray)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/

