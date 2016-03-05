/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan Cornet
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "TerminateGraphics.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "sci_malloc.h"
#include "getDictionaryGetProperties.h"
#include "getDictionarySetProperties.h"
#include "graphicModuleLoad.h"
/*--------------------------------------------------------------------------*/

/*
  We need to add "manually" the functions set, get and delete because
  if the module graphics is not loaded and because set, get and delete
  are essentially functions from the graphics module, they will not be
  available in NWNI mode and if Scilab has been compiled with the flag
  --without-gui.
*/

#define DELETE_INDEX 62
#define GET_INDEX    63
#define SET_INDEX    64

/*--------------------------------------------------------------------------*/
void loadGraphicModule(void)
{

}

/*--------------------------------------------------------------------------*/
BOOL TerminateGraphics(void)
{
    return TRUE;
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
unsigned long getHandleFromStack(size_t stackPointer)
{
    return (unsigned long)(-1);
}

/*--------------------------------------------------------------------------*/
int callSetProperty(void* _pvCtx, int pObjUID, void* _pvData, int valueType, int nbRow, int nbCol, const char * propertyName)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitScreenPosition(int pobj, int pposx, int pposy)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
char *getStringFromStack(size_t stackPointer)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
double *getDoubleMatrixFromStack(size_t stackPointer)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
double getDoubleFromStack(size_t stackPointer)
{
    return 0.;
}

/*--------------------------------------------------------------------------*/
int getFigureFromIndex(int figNum)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
char **getStringMatrixFromStack(size_t stackPointer)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int allocateText(int pparentsubwinUID,
                 char **text,
                 int nbRow,
                 int nbCol,
                 double x,
                 double y,
                 BOOL autoSize,
                 double userSize[2],
                 int centerPos, int *foreground, int *background, BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
int sciInitBackground(int pobj, int colorindex)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitMarkBackground(int pobj, int colorindex)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciInitMarkForeground(int pobj, int colorindex)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciGetNumColors(int pobjUID)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void sciGetViewingArea(int pObjUID, int *xPos, int *yPos, int *width, int *height)
{

}

/*--------------------------------------------------------------------------*/
int ComputeXIntervals(int pobjUID, char xy_type, double **vector, int *N, int checkdim)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
void deleteMatrix(sciMatrix * mat)
{

}

/*--------------------------------------------------------------------------*/
char *getStrMatElement(const StringMatrix * mat, int row, int col)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
StringMatrix *computeDefaultTicsLabels(int pobjUID)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
void rotate2D(double from[2], double center[2], double angle, double dest[2])
{

}

/*--------------------------------------------------------------------------*/
void stringArrayCopy(char *dest[], char *src[], int nbElement)
{

}

/*--------------------------------------------------------------------------*/
void sciGetLogFlags(int pObjUID, char flags[3])
{

}

/*--------------------------------------------------------------------------*/
BOOL sciisTextEmpty(int iIdentifier)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
int getMatNbCol(const sciMatrix * mat)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int getMatNbRow(const sciMatrix * mat)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
char **getStrMatData(const StringMatrix * mat)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
BOOL isFigureModel(int pObjUID)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
int getPixelModeIndex(const char *modeName)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
int sciSetXorMode(int pobj, int value)
{
    return -1;
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
void vectAdd3D(const double v1[3], const double v2[3], double res[3])
{

}

/*--------------------------------------------------------------------------*/
int ConstructPolyline(int pparentsubwin, double *pvecx, double *pvecy, double *pvecz,
                      int closed, int n1, int plot, int *foreground, int *background,
                      int *mark_style, int *mark_foreground, int *mark_background, BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/
BOOL isPointInTriangle(const double point[2], const double a[2], const double b[2], const double c[2])
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
void vectSubstract3D(const double v1[3], const double v2[3], double res[3])
{

}

/*--------------------------------------------------------------------------*/
void normalize3D(double vect[3])
{

}

/*--------------------------------------------------------------------------*/
void scalarMult3D(const double v[3], double scalar, double res[3])
{

}

/*--------------------------------------------------------------------------*/
void ChoixFormatE(char *fmt, double xmin, double xmax, double xpas)
{

}

/*--------------------------------------------------------------------------*/
int GradLog(double _min, double _max, double *_grads, int *n_grads, int compNgrads)
{
    return -1;
}

/*--------------------------------------------------------------------------*/
// FIXME : Kill me
void intArrayCopy(int dest[], const int src[], int nbElement)
{

}

/*--------------------------------------------------------------------------*/
void doubleArrayCopy(double dest[], const double src[], int nbElement)
{

}

/*--------------------------------------------------------------------------*/
// FIXME : Kill me
void setDoubleArraySingleValue(double dest[], double value, int nbElement)
{

}

/*--------------------------------------------------------------------------*/
BOOL sciIsExistingFigure(int figNum)
{
    return FALSE;
}

/*--------------------------------------------------------------------------*/
void cloneAxesModel(int pstFigureUID)
{
}

/*--------------------------------------------------------------------------*/
int createNewFigureWithAxes(void)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
int getOrCreateDefaultSubwin(void)
{
    return 0;
}

/*--------------------------------------------------------------------------*/

