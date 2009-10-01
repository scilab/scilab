/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan Cornet
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "graphicModuleLoad.h"
#include "TerminateGraphics.h"
#include "getDictionaryGetProperties.h"
#include "getDictionarySetProperties.h"
#include "HandleManagement.h"
#include "DrawObjects.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "math_graphics.h"
#include "DestroyObjects.h"
#include "axesScale.h"
#include "BuildObjects.h"
#include "BasicAlgos.h"
#include "pixel_mode.h"
#include "Axes.h"
#include "getPropertyAssignedValue.h"
#include "SetHashTable.h"
#include "WindowList.h"
#include "Scierror.h"
#include "localization.h"
#include "Format.h"
#include "MALLOC.h"
#include "Interaction.h"
#include "InitObjects.h"
#include "ObjectStructure.h"
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
int sciSetViewport(  sciPointObj * pObj, const int viewport[4] )
{
	return 0;
}
/*--------------------------------------------------------------------------*/
void sciGetViewport( sciPointObj * pObj, int viewport[4] )
{

}
/*--------------------------------------------------------------------------*/
double Maxi(const double vect[], int n)
{
	return 0.0;
}
/*--------------------------------------------------------------------------*/
double Mini(const double vect[], int n)
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
int sciGetHeight (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetNum (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
BOOL sciAddThisToItsParent (sciPointObj * pthis, sciPointObj * pparent)
{
	return FALSE ;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentFigure( void )
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
int sciAddNewHandle (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void createDefaultRelationShip(sciPointObj * pObj)
{

}
/*--------------------------------------------------------------------------*/
int sciSetEntityType (sciPointObj * pobj, sciEntityType value)
{
	return -1 ;
}
/*--------------------------------------------------------------------------*/
sciPointObj * sciStandardBuildOperations( sciPointObj * pObj, sciPointObj * parent ) 
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
void sciInitSelectedSons( sciPointObj * pObj )
{

}
/*--------------------------------------------------------------------------*/
sciPointObj *sciGetParent (sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
sciPointObj *sciGetPointerFromHandle (long handle)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
unsigned long getHandleFromStack( size_t stackPointer )
{
	return 	-1;
}
/*--------------------------------------------------------------------------*/
int callSetProperty( sciPointObj * pObj, size_t stackPointer, int valueType, int nbRow, int nbCol, char * propertyName )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
sciEntityType sciGetEntityType (sciPointObj * pobj)
{
	return (sciEntityType)0;
}
/*--------------------------------------------------------------------------*/
int sciStandardDestroyOperations( sciPointObj * pThis ) 
{
	return -1;
}
/*--------------------------------------------------------------------------*/
sciPointObj *sciGetParentFigure (sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
int sciGetWidth (sciPointObj * pobj)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
int sciSetDimension(  sciPointObj * pobj, int newWidth, int newHeight )
{
	return -1 ;
}
/*--------------------------------------------------------------------------*/
int sciInitScreenPosition(sciPointObj * pobj, int pposx, int pposy)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
BOOL sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
char * getStringFromStack( size_t stackPointer )
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
double * getDoubleMatrixFromStack( size_t stackPointer ) 
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
double getDoubleFromStack( size_t stackPointer ) 
{
	return 0.;
}
/*--------------------------------------------------------------------------*/
sciPointObj * getFigureFromIndex(int figNum)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
char ** getStringMatrixFromStack( size_t stackPointer )
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
sciPointObj *sciGetPointerFromJavaIndex (int javaIndex)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
long sciGetHandle (sciPointObj * pobj)
{
	return (long)0;
}
/*--------------------------------------------------------------------------*/
int sciInitForeground( sciPointObj * pobj, int colorindex )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitFontForeground(sciPointObj * pobj, int colorindex ) 
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetFontForeground (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitFontSize(sciPointObj * pobj, double fontSize)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
sciPointObj * allocateText( sciPointObj        * pparentsubwin,
										   char             ** text          ,
										   int                 nbRow         ,
										   int                 nbCol         ,
										   double              x             ,
										   double              y             ,
										   BOOL                autoSize      ,
										   double              userSize[2]   ,
										   BOOL                centerPos     ,
										   int               * foreground    ,
										   int               * background    , 
										   BOOL                isboxed       ,
										   BOOL                isline        ,
										   BOOL                isfilled      ,
										   sciTextAlignment    align          )
{
    return NULL;
}
/*--------------------------------------------------------------------------*/
int sciGetForeground (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetBackground (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitIsFilled(sciPointObj * pobj, BOOL isfilled)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitIsLine(sciPointObj * pobj, BOOL isline)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitLineStyle( sciPointObj * pobj, int linestyle )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitLineWidth( sciPointObj * pobj, double linewidth )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitBackground( sciPointObj * pobj, int colorindex ) 
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitMarkStyle(sciPointObj * pobj, int markstyle)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitIsMark(sciPointObj * pobj, BOOL ismark)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitMarkBackground(sciPointObj * pobj, int colorindex)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetMarkBackground (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitMarkForeground(sciPointObj * pobj, int colorindex)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetMarkForeground (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitMarkSizeUnit(sciPointObj * pobj, int marksizeunit)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetNbTypedObjects( sciPointObj * pObj, sciEntityType type )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
sciPointObj * allocatePolyline(sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
							   int closed, int n1,int plot, int *foreground, int *background,
							   int *mark_style, int *mark_foreground, int *mark_background,
							   BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
int sciInitArrowSize( sciPointObj * pobj, double size )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitPolylineStyle( sciPointObj * pobj, int plot )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetNumColors (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int deallocateText( sciPointObj * pthis )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitFontBackground(sciPointObj * pobj, int color)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void sciGetLegendPos (sciPointObj * pobj, double position[2])
{

}
/*--------------------------------------------------------------------------*/
char sciGetxLocation(sciPointObj * pObj)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
char sciGetyLocation(sciPointObj * pObj)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
sciLegendPlace sciGetLegendPlace (sciPointObj * pobj)
{
	return (sciLegendPlace)0;
}
/*--------------------------------------------------------------------------*/
void sciGetDataBounds( sciPointObj * pObj, double bounds[6] )
{

}
/*--------------------------------------------------------------------------*/
void sciGetViewingArea(sciPointObj * pObj, int * xPos, int * yPos, int * width, int * height)
{

}
/*--------------------------------------------------------------------------*/
void destroyGraphicPointer(void * pointer)
{

}
/*--------------------------------------------------------------------------*/
int ComputeXIntervals( sciPointObj * pobj, char xy_type, double ** vector, int * N, int checkdim )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void deleteMatrix( sciMatrix * mat ) 
{

}
/*--------------------------------------------------------------------------*/
char * getStrMatElement( const StringMatrix * mat, int row, int col )
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
StringMatrix * computeDefaultTicsLabels( sciPointObj * pobj )
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
void rotate2D( double from[2], double center[2], double angle, double dest[2] )
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetAutoRotation ( sciPointObj * pObj ) 
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL needsDisplay(sciPointObj * pFigure)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsAutoDrawable( sciPointObj * pobj ) 
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void stringArrayCopy( char * dest[], char * src[], int nbElement )
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetVisibility (sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void sciGetLogFlags(sciPointObj * pObj, char flags[3])
{

}
/*--------------------------------------------------------------------------*/
int sciSetFontOrientation (sciPointObj * pobj, double textorientation)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetAutoPosition ( sciPointObj * pObj )
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int sciSetFontStyle (sciPointObj * pobj, int iAttributes )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciInitIsClipping(sciPointObj * pobj, int value)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciSetLegendPos (sciPointObj * pobj, double position[])
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciSetClipping (sciPointObj * pobj, double pclip[4] )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciSetTextPos( sciPointObj * pObj, double posX, double posY, double posZ)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void sciGetUserSize( sciPointObj * pObj, double * width, double * height )
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsBoxed (sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsLine (sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetAutoSize( sciPointObj * pObj )
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetCenterPos( sciPointObj * pObj )
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciisTextEmpty( sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
double sciGetFontOrientation (sciPointObj * pobj)
{
	return 0.;
}
/*--------------------------------------------------------------------------*/
sciTextAlignment sciGetAlignment( sciPointObj * pObj ) 
{
	return (sciTextAlignment)0;
}
/*--------------------------------------------------------------------------*/
StringMatrix * sciGetText (sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
int getMatNbCol( const sciMatrix * mat )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int getMatNbRow( const sciMatrix * mat )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
char ** getStrMatData(  const StringMatrix * mat ) 
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetUseNurbs(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL isFigureModel(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsUsingFractionalMetrics(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
sciFont *sciGetFontContext (sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
double sciGetFontSize(sciPointObj * pobj)
{
	return 0.;
}
/*--------------------------------------------------------------------------*/
 int sciGetFontStyle (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3])
{

}
/*--------------------------------------------------------------------------*/
void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3])
{

}
/*--------------------------------------------------------------------------*/
void sciGetGridStyle( sciPointObj * pObj, int * xStyle, int * yStyle, int * zStyle )
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetAutoSubticks(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetGridFront(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIs3d( sciPointObj * pObj )
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int sciGetSubwinIndex(sciPointObj * pSubwin)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
EAxesBoxType sciGetBoxType( sciPointObj * pobj ) 
{
	return (EAxesBoxType)0;
}
/*--------------------------------------------------------------------------*/
void sciGetTextPos(sciPointObj * pObj, double position[3])
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetRealVisibility (sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int getPixelModeIndex( const char * modeName )
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciSetXorMode (sciPointObj * pobj, int value)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciSetVisibility (sciPointObj * pobj, BOOL value)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6])
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetZooming (sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetTightLimitsOn(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int sciSetRealDataBounds(sciPointObj * pObj, const double bounds[6])
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void mat4DMult(const double mat4D[4][4], const double vect3D[3], double res[3])
{

}
/*--------------------------------------------------------------------------*/
void setToIdentity(double mat4D[4][4])
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsIsoView(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void vectAdd3D(const double v1[3], const double v2[3], double res[3])
{

}
/*--------------------------------------------------------------------------*/
sciPointObj *ConstructPolyline (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
								int closed, int n1, int plot, int *foreground, int *background,
								int *mark_style, int *mark_foreground, int *mark_background,
								BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
int sciSetIsClipping (sciPointObj * pobj, int value)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
BOOL isPointInTriangle(const double point[2], const double a[2],
					   const double b[2], const double c[2])
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void vectSubstract2D(const double vect1[2], const double vect2[], double res[2])
{

}
/*--------------------------------------------------------------------------*/
void scalarMult2D(const double v[2], const double scalar, double res[2])
{

}
/*--------------------------------------------------------------------------*/
void vectAdd2D(const double v1[2], const double v2[2], double res[2])
{

}
/*--------------------------------------------------------------------------*/
int sciSetZoomBox(sciPointObj * pObj, const double zoomBox[6])
{
	return -1;
}
/*--------------------------------------------------------------------------*/
double *sciGetMargins( sciPointObj * pObj )
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
double * sciGetAxesBounds( sciPointObj * pObj )
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
void sciGetViewingAngles( sciPointObj * pObj, double * alpha, double * theta) 
{

}
/*--------------------------------------------------------------------------*/
void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3])
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsCubeScaled(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void vectSubstract3D(const double v1[3], const double v2[3], double res[3])
{

}
/*--------------------------------------------------------------------------*/
void normalize3D( double vect[3] )
{

}
/*--------------------------------------------------------------------------*/
void scalarMult3D(const double v[3], double scalar, double res[3])
{

}
/*--------------------------------------------------------------------------*/
int TheTicks( double * xminv ,
			 double * xmaxv , 
			 double * grads , 
			 int    * ngrads,
			 int      compNgrads ) 
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void ChoixFormatE(char *fmt, double xmin, double xmax, double xpas)
{

}
/*--------------------------------------------------------------------------*/
int GradLog( double _min, double _max, double *_grads, int * n_grads, int compNgrads)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, const double * grads, int nbsubtics_input)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetMarkStyle (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetMarkSize (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetMarkSizeUnit (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetLineStyle (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
double sciGetLineWidth (sciPointObj * pobj)
{
	return 0.;
}
/*--------------------------------------------------------------------------*/
double sciGetArrowSize (sciPointObj * pobj)
{
	return 0.;
}
/*--------------------------------------------------------------------------*/
sciGraphicContext *sciGetGraphicContext (sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
int * sciGetInterpVector(sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
void intArrayCopy( int dest[], const int src[], int nbElement )
{

}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsClosed(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsFilled (sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsColorInterpolated(sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsDisplayingLines(sciPointObj * pObj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciGetIsMark (sciPointObj * pobj)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int sciGetNbPoints(sciPointObj * pObj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void doubleArrayCopy( double dest[], const double src[], int nbElement )
{

}
/*--------------------------------------------------------------------------*/
void setDoubleArraySingleValue( double dest[], double value, int nbElement ) 
{

}
/*--------------------------------------------------------------------------*/
int sciGetPolylineStyle(sciPointObj * pObj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
void unzoomSubwin(sciPointObj * pSubwin)
{

}
/*--------------------------------------------------------------------------*/
void sciZoomObject(sciPointObj * pObj, int x1, int y1, int x2, int y2)
{

}
/*--------------------------------------------------------------------------*/
sciPointObj * getClickedSubwin(sciPointObj * pFigure, int xCoord, int yCoord)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
void updateViewingAngles(sciPointObj * pSubwin, double deltaAlpha, double deltaTheta)
{

}
/*--------------------------------------------------------------------------*/
int deallocatePolyline (sciPointObj * pthis)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
int sciGetXorMode (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
sciSons *sciGetLastSons (sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj)
{
	return TRUE;
}
/*--------------------------------------------------------------------------*/
int sciGetIsClipping (sciPointObj * pobj)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
sciPointObj *sciGetParentSubwin (sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
void sciGetRealDataBounds( sciPointObj * pObj, double bounds[6] )
{

}
/*--------------------------------------------------------------------------*/
int sciInitMarkSize(sciPointObj * pobj, int marksize)
{
	return -1;
}
/*--------------------------------------------------------------------------*/
double *sciGetClipping (sciPointObj * pobj)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL sciIsExistingFigure(int figNum)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
