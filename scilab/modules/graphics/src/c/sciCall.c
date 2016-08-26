/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2014-2015 - Scilab Enterprises - Calixte DENIZET
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

/*------------------------------------------------------------------------
 *    Graphic library
 *    Graphic subroutines interface
 --------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "SetProperty.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "BasicAlgos.h"
#include "math_graphics.h"
#include "PloEch.h"
#include "Axes.h"
#include "Fec.h"
#include "GrayPlot.h"
#include "localization.h"
#include "sci_malloc.h" /* MALLOC */
#include "Scierror.h"
#include "sciCall.h"
#include "Plot2d.h"
#include "HandleManagement.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"
#include "Format.h"
#include "deleteGraphicObject.h"

/** Check if auto_scale is on/off */
static int mustUpdate(int iSubwinUID);

/** Update data bounds according to the given data bound passed in rect */
static void updateXYDataBounds(int iSubwinUID, double rect[6]);
static void updateXYZDataBounds(int iSubwinUID, double rect[6]);

/*------------------------------------------------
 * Objrect :
 * On recupere la figure courante, puis on recupere la sous fenetre qui y est selectionnee
 *   puis on contruit le rectangle, qu on le place comme objet courant
 * ensuite il reste qu'appeler la fonction du dessin de l'objet
 *-----------------------------------------------*/

void Objrect (double* x         ,
              double* y         ,
              double* width     ,
              double* height    ,
              int    * foreground,
              int    * background,
              BOOL     isfilled  ,
              BOOL     isline    ,
              long   * hdl )
{
    int iNewObjUID = 0;
    int iSubwinUID = 0;

    iSubwinUID = getCurrentSubWin();

    /* check if the auto_clear property is on and then erase everything */
    checkRedrawing();

    if (mustUpdate(iSubwinUID))
    {
        double rect[6];
        rect[0] = *x;
        rect[1] = *x + *width;
        rect[2] = *y - *height;
        rect[3] = *y;

        updateXYDataBounds(iSubwinUID, rect);
    }

    /*newObjUID = ConstructRectangle(iSubwinUID , *x, *y, *height, *width,
      foreground, background, isfilled, isline);*/

    iNewObjUID = createRect(iSubwinUID, *x, *y, *height, *width,
                            foreground == NULL ? -1 : *foreground,
                            background == NULL ? -1 : *background,
                            (int)isfilled, (int)isline);

    if (iNewObjUID == 0)
    {
        /* an error occurred */
        *hdl = -1;
        return;
    }

    setCurrentObject(iNewObjUID);
    *hdl = getHandle(iNewObjUID);
}


/*----------------------------------------------
 * Objarc :
 *-----------------------------------------------*/

void Objarc(double* angle1    ,
            double* angle2    ,
            double* x         ,
            double* y         ,
            double* width     ,
            double* height    ,
            int    * foreground,
            int    * background,
            BOOL     isfilled  ,
            BOOL     isline    ,
            long   * hdl  )
{
    int iSubwinUID = 0;
    int iObjUID = 0;

    iSubwinUID = getCurrentSubWin();
    checkRedrawing();

    if (mustUpdate(iSubwinUID))
    {
        double rect[6];
        const double two_pi = 2 * M_PI;
        if (abs(*angle2) >= two_pi)
        {
            rect[0] = *x;
            rect[1] = *x + *width;
            rect[2] = *y - *height;
            rect[3] = *y;
        }
        else
        {
            double a = *angle1;
            double s = *angle2;
            double a1, b1;
            double b;

            a -= (floor(a / two_pi)) * two_pi;
            b = a + s;

            if (s >= 0)
            {
                b = a + s;
            }
            else
            {
                b = a;
                a += s;
            }

            b1 = b / M_PI;
            a1 = a / M_PI;

            // is there a 2k\pi in [a,b] ?
            if (ceil(a1 / 2) <= floor(b1 / 2))
            {
                rect[1] = *x + *width;
            }
            else
            {
                rect[1] = *x + 0.5 * *width * (1 + Max(cos(a), cos(b)));
            }

            // is there a (2k+1)\pi in [a,b] ?
            if (ceil((a1 - 1) / 2) <= floor((b1 - 1) / 2))
            {
                rect[0] = *x;
            }
            else
            {
                rect[0] = *x + 0.5 * *width * (1 + Min(cos(a), cos(b)));
            }

            // is there a (2k+1/2)\pi in [a,b] ?
            if (ceil((a1 - 0.5) / 2) <= floor((b1 - 0.5) / 2))
            {
                rect[3] = *y;
            }
            else
            {
                rect[3] = *y + 0.5 * *height * (-1 + Max(sin(a), sin(b)));
            }

            // is there a (2k+3/2)\pi in [a,b] ?
            if (ceil((a1 - 1.5) / 2) <= floor((b1 - 1.5) / 2))
            {
                rect[2] = *y - *height;
            }
            else
            {
                rect[2] = *y + 0.5 * *height * (-1 + Min(sin(a), sin(b)));
            }
        }

        updateXYDataBounds(iSubwinUID, rect);
    }

    iObjUID = createArc(iSubwinUID, *x, *y,
                        *height, *width, *angle1, *angle2, foreground, background, isfilled, isline);

    setCurrentObject(iObjUID);
    *hdl = getHandle(iObjUID);
}

/*------------------------------------------------
 * Objpoly :
 *-----------------------------------------------*/

void Objpoly (double  * x     ,
              double  * y     ,
              int   n     ,
              int   closed,
              int       mark  ,
              long    * hdl)
{
    int iSubwinUID = 0;
    int iObjUID = 0;

    iSubwinUID = getCurrentSubWin();

    checkRedrawing();

    if (n && mustUpdate(iSubwinUID))
    {
        double rect[6];
        MiniMaxi(x, n, rect, rect + 1);
        MiniMaxi(y, n, rect + 2, rect + 3);

        updateXYDataBounds(iSubwinUID, rect);
    }

    if (mark <= 0)
    {
        int absmark = abs(mark);
        iObjUID = ConstructPolyline(iSubwinUID, x, y, PD0, closed, n, 1,
                                    NULL, NULL, &absmark, NULL, NULL, FALSE, FALSE, TRUE, FALSE);
    }
    else
    {
        iObjUID = ConstructPolyline(iSubwinUID, x, y, PD0, closed, n, 1,
                                    &mark, NULL, NULL, NULL, NULL, TRUE, FALSE, FALSE, FALSE);
    }

    if (iObjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objpoly");
        return;
    }

    setCurrentObject(iObjUID);
    *hdl = getHandle(iObjUID);
}


/*------------------------------------------------
 * Objfpoly :
 *-----------------------------------------------*/

void Objfpoly (double  * x    ,
               double  * y    ,
               int   n    ,
               int * style,
               long    * hdl  ,
               int   shading)
{
    int iSubwinUID = 0;
    int iObjUID = 0;

    int fillcolor = 0;
    int contourcolor = 0;
    int *piContourColor = &contourcolor;

    int closed = 1; /* we close the polyline by default */

    iSubwinUID = getOrCreateDefaultSubwin();

    checkRedrawing();

    if (n && mustUpdate(iSubwinUID))
    {
        double rect[6];
        MiniMaxi(x, n, rect, rect + 1);
        MiniMaxi(y, n, rect + 2, rect + 3);

        updateXYDataBounds(iSubwinUID, rect);
    }

    if (shading == 2)
    {
        /* interpolated shading is "on" */
        iObjUID = ConstructPolyline(iSubwinUID, x, y, PD0, closed, n,
                                    1, NULL, style, NULL, NULL, NULL, FALSE, TRUE, FALSE, TRUE);
    }
    else
    {

        /* flat mode is "on" */
        if (*style < 0)
        {
            fillcolor = abs(*style);
            iObjUID = ConstructPolyline(iSubwinUID, x, y, PD0, closed, n,
                                        1, NULL, &fillcolor, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE);
        }
        else if (*style == 0)
        {
            getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piContourColor);
            iObjUID = ConstructPolyline(iSubwinUID, x, y, PD0, closed, n,
                                        1, &contourcolor, NULL, NULL, NULL, NULL, TRUE, FALSE, FALSE, FALSE);
        }
        else
        {
            /* *style > 0*/
            fillcolor = *style;
            getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piContourColor);
            iObjUID = ConstructPolyline(iSubwinUID, x, y, PD0, closed, n,
                                        1, &contourcolor, &fillcolor, NULL, NULL, NULL, TRUE, TRUE, FALSE, FALSE);
        }

    }

    if (iObjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objfpoly");
        return;
    }

    setCurrentObject(iObjUID);
    *hdl = getHandle(iObjUID);
}


/*-----------------------------------------------------------
 *   Objsegs :
 *-----------------------------------------------------------*/
void Objsegs (int * style,
              int   flag ,
              int   n1   ,
              double  * x    ,
              double  * y    ,
              double  * z    ,
              double    arsize)
{
    int iObjUID = 0;
    int iSubwinUID = 0;
    int type = 0, colored = 0;
    double *fx = NULL, *fy = NULL; // No fx or fy
    int typeofchamp = -1; /* no champ here, only segs ; this info is useless */

    checkRedrawing();
    iSubwinUID = getCurrentSubWin();

    if (n1 && mustUpdate(iSubwinUID))
    {
        double rect[6];
        MiniMaxi(x, n1, rect, rect + 1);
        MiniMaxi(y, n1, rect + 2, rect + 3);

        if (z)
        {
            MiniMaxi(z, n1, rect + 4, rect + 5);
            updateXYZDataBounds(iSubwinUID, rect);
        }
        else
        {
            updateXYDataBounds(iSubwinUID, rect);
        }
    }

    iObjUID = createSegs(iSubwinUID, x, n1, y, n1, z, (z == NULL ? 0 : n1), style, flag == 0 ? 1 : n1, arsize);

    if (iObjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objsegs");
        return;
    }

    setCurrentObject(iObjUID);
}
/*-----------------------------------------------------------
 * Objstring:
 *-----------------------------------------------------------*/

/* box is an OUTPUT re-used inside matdes.c in scixstring */
void Objstring(char            ** fname      ,
               int                nbRow      ,
               int                nbCol      ,
               double             x          ,
               double             y          ,
               double           * angle      ,
               double             box[4]     ,
               BOOL               autoSize   ,
               double             userSize[2],
               long             * hdl        ,
               int                centerPos  ,
               int              * foreground ,
               int              * background ,
               BOOL               isboxed    ,
               BOOL               isline     ,
               BOOL               isfilled   ,
               sciTextAlignment   alignment)
{
    int iSubwinUID = 0;
    int iObjUID = 0;

    iSubwinUID = getCurrentSubWin();

    checkRedrawing();

    iObjUID = ConstructText(iSubwinUID   ,
                            fname     ,
                            nbRow     ,
                            nbCol     ,
                            x         ,
                            y         ,
                            autoSize  ,
                            userSize  ,
                            centerPos ,
                            foreground,
                            background,
                            isboxed   ,
                            isline    ,
                            isfilled  ,
                            alignment);

    if (iObjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objstring");
        return;
    }

    *hdl = getHandle(iObjUID);

    setGraphicObjectProperty(iObjUID, __GO_FONT_ANGLE__, angle, jni_double, 1);
}


/*------------------------------------------------
 *  plot2d
 *-----------------------------------------------*/

void Objplot2d (int       ptype     ,
                char      logflags[],
                double    x[]       ,
                double    y[]       ,
                int * n1        ,
                int * n2        ,
                int   style[]   ,
                char      strflag[] ,
                char      legend[]  ,
                double    brect[]   ,
                int   aaint[]   ,
                BOOL      flagNax)
{
    plot2dn(ptype, logflags, x, y, n1, n2, style, strflag, legend, brect, aaint, flagNax, 4L, bsiz);
}

/*------------------------------------------------
 *  grayplot
 *-----------------------------------------------*/
void Objgrayplot (double    x[]      ,
                  double    y[]      ,
                  double    z[]      ,
                  int * n1       ,
                  int * n2       ,
                  char      strflag[],
                  double    brect[]  ,
                  int   aaint[]  ,
                  BOOL      flagNax,
                  char logflag[])
{
    C2F(xgray)(x, y, z, n1, n2, strflag, brect, aaint, flagNax, logflag, bsiz);
}

/*------------------------------------------------
 *  Matplot
 *-----------------------------------------------*/
void Objmatplot (double    z[]      ,
                 int * n1       ,
                 int * n2       ,
                 char      strflag[],
                 double    brect[]  ,
                 int    aaint[]  ,
                 BOOL      flagNax)
{
    C2F(xgray1)(z, n1, n2, strflag, brect, aaint, flagNax, bsiz);
}

void ObjmatplotImage (void * z      ,
                      int * n1       ,
                      int * n2       ,
                      char      strflag[],
                      double    brect[]  ,
                      int    aaint[]  ,
                      BOOL      flagNax,
                      int plottype)
{
    if (plottype == -1)
    {
        C2F(xgray1)((double *)z, n1, n2, strflag, brect, aaint, flagNax, bsiz);
    }
    else
    {
        C2F(implot)((unsigned char *)z, n1, n2, strflag, brect, aaint, flagNax, bsiz, plottype);
    }
}

/*------------------------------------------------
 *  Matplot1
 *-----------------------------------------------*/
void Objmatplot1 (double    z[],
                  int * n1 ,
                  int * n2 ,
                  double xrect[], int plottype)
{
    if (plottype == -1)
    {
        C2F(xgray2)(z, n1, n2, xrect);
    }
    else
    {
        C2F(implot1)((unsigned char *)z, n1, n2, xrect, plottype);
    }
}

/*------------------------------------------------
 *  plot3d
 *-----------------------------------------------*/
void Objplot3d (char    * fname ,
                int * isfac ,
                int * izcol ,
                double    x[]   ,
                double    y[]   ,
                double    z[]   ,
                double  * zcol  ,
                int * m     ,
                int * n     ,
                double  * theta ,
                double  * alpha ,
                char    * legend,
                int * iflag ,
                double  * ebox  ,
                int * m1    , /*Adding F.Leray 12.03.04 and 19.03.04*/
                int * n1    ,
                int * m2    ,
                int * n2    ,
                int * m3    ,
                int * n3    ,
                int * m3n   ,
                int * n3n)
/* F.Leray 25.04.05 : warning here legends means "X@Y@Z": it is labels writings!! */
/* legends has not the same meaning than inside plot2dn (there, it is really the legends of the plotted curves)*/
{
    sciTypeOf3D typeof3d;
    int flagcolor = 0;
    int* pObj = NULL;
    int i = 0;

    int iSubwinUID = 0;

    int firstPlot = 0;

    int clipState = 0;

    int iNewSurfaceUID = 0;


    /* =================================================
     * Force SubWindow properties according to arguments
     * ================================================= */

    iSubwinUID = getCurrentSubWin();
    checkRedrawing();
    initSubWinTo3d(iSubwinUID, legend, iflag, *alpha, *theta, ebox, x, *m1 **n1, y, *m2 **n2, z, *m3 **n3);

    /* =================================================
     * Analyze arguments to find entity type
     * ================================================= */

    if (*isfac == 1)
    {
        if (*izcol == 0)
        {
            if (strcmp(fname, "plot3d1") == 0)
            {
                typeof3d = SCI_FAC3D;
                flagcolor = 1;
            }
            else
            {
                typeof3d = SCI_FAC3D;
                flagcolor = 0;
            }
        }
        else if (*izcol == 2)
        {
            typeof3d = SCI_FAC3D;
            flagcolor = 3;
        }
        else
        {
            typeof3d = SCI_FAC3D;
            flagcolor = 2;
        }
    }
    else if (*isfac == 0)
    {
        if (strcmp(fname, "plot3d1") == 0)
        {
            typeof3d = SCI_PLOT3D;
            flagcolor = 1;
        }
        else
        {
            typeof3d = SCI_PLOT3D;
            flagcolor = 0;
        }
    }
    else
    {
        typeof3d = SCI_PARAM3D1;
        flagcolor = 1;
    }

    /* =================================================
     * Construct the Entities
     * ================================================= */

    /*Distinction here between SCI_PARAM3D1 and others*/
    if (typeof3d != SCI_PARAM3D1)
    {
        int dimvectx = -1;
        int dimvecty = -1;
        if (*isfac == 1)
        {
            /* x is considered as a matrix */
            dimvectx = -1;
        }
        else if (*m1 == 1) /* x is a row vector */
        {
            dimvectx = *n1;
        }
        else if (*n1 == 1) /* x is a column vector */
        {
            dimvectx = *m1;
        }
        else /* x is a matrix */
        {
            dimvectx = -1;
        }

        if (dimvectx > 1)
        {
            int monotony = checkMonotony(x, dimvectx);
            if (monotony == 0)
            {
                Scierror(999, _("%s: x vector is not monotonous.\n"), "Objplot3d");
                return;
            }

        }

        if (*isfac == 1)
        {
            /* y is considered as a matrix */
            dimvecty = -1;
        }
        else if (*m2 == 1) /* y is a row vector */
        {
            dimvecty = *n2;
        }
        else if (*n2 == 1) /* y is a column vector */
        {
            dimvecty = *m2;
        }
        else /* y is a matrix */
        {
            dimvecty = -1;
        }

        if (dimvecty > 1)
        {
            /* test the monotony on y*/
            int monotony = checkMonotony(y, dimvecty);
            if (monotony == 0)
            {
                Scierror(999, _("%s: y vector is not monotonous.\n"), "Objplot3d");
                return;
            }
        }

        iNewSurfaceUID = ConstructSurface(iSubwinUID, typeof3d,
                                          x, y, z, zcol, *izcol, *m, *n, iflag, ebox, flagcolor,
                                          isfac, m1, n1, m2, n2, m3, n3, m3n, n3n);

        if (iNewSurfaceUID == 0)
        {
            Scierror(999, _("%s: No more memory.\n"), "Objplot3d");
            return;
        }

        setCurrentObject(iNewSurfaceUID);

        /* Force clipping, 1: CLIPGRF */
        clipState = 1;
        setGraphicObjectProperty(iNewSurfaceUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);
    }
    else
    {
        int iNewPolylineUID = 0;
        int iCurrentSubwinUID = 0;

        if ((pObj = (int*)MALLOC (*n * sizeof (int))) == NULL)
        {
            Scierror(999, "%s: No more memory.\n", fname);
            return;
        }

        iCurrentSubwinUID = getCurrentSubWin();

        for (i = 0; i < *n; ++i)
        {
            if ((*n > 0) && (zcol != (double *)NULL))
            {
                if ((int) zcol[i] > 0)
                {
                    int intzcol = (int) zcol[i];
                    iNewPolylineUID = ConstructPolyline
                                      (iCurrentSubwinUID,
                                       &(x[*m * i]), &(y[*m * i]), &(z[*m * i]), 0, *m, 1,
                                       &intzcol, NULL, NULL, NULL, NULL, TRUE, FALSE, FALSE, FALSE);
                }
                else
                {
                    int intzcol = (int) - zcol[i];
                    iNewPolylineUID = ConstructPolyline
                                      (iCurrentSubwinUID,
                                       &(x[*m * i]), &(y[*m * i]), &(z[*m * i]), 0, *m, 1,
                                       NULL, NULL, &intzcol, NULL, NULL, FALSE, FALSE, TRUE, FALSE);
                }
            }
            else
            {
                /* default case, nothing is given */
                int curcolor = 0;
                int *piCurColor = &curcolor;

                getGraphicObjectProperty(iCurrentSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piCurColor);

                iNewPolylineUID = ConstructPolyline(iCurrentSubwinUID,
                                                    &(x[*m * i]), &(y[*m * i]), &(z[*m * i]), 0, *m, 1,
                                                    &curcolor, NULL, NULL, NULL, NULL, TRUE, FALSE, FALSE, FALSE);
            }

            if (iNewPolylineUID == 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                FREE(pObj);
                return;
            }

            setCurrentObject(iNewPolylineUID);
            setGraphicObjectRelationship(iCurrentSubwinUID, iNewPolylineUID);

            /* Force clipping, 1: CLIPGRF */
            clipState = 1;
            setGraphicObjectProperty(iNewPolylineUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

            pObj[i] = iNewPolylineUID;
        }

        /** construct Compound and make it current object**/
        if (*n > 1)
        {
            int o = createCompound (iCurrentSubwinUID, pObj, *n);
            setCurrentObject(o);
        }
        FREE(pObj);
    }

    /* =================================================
     * Redraw Figure
     * ================================================= */

    // subwin has been modified

    firstPlot = 0;
    setGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);
}
/*-----------------------------------------------------------
 * Objaxis:
 *-----------------------------------------------------------*/

void Objdrawaxis (char     dir    ,
                  char     tics   ,
                  double* x      ,
                  int    * nx     ,
                  double* y      ,
                  int    * ny     ,
                  char   * val[]  ,
                  int      subint ,
                  char   * format ,
                  int      font   ,
                  int      textcol,
                  int      ticscol,
                  char     flag   ,
                  int      seg    ,
                  int      nb_tics_labels)
{
    int iObjUID = 0;
    int iSubwinUID = 0;
    int ticksDirection = 0;
    int ticksStyle = 0;

    iSubwinUID = getCurrentSubWin();

    checkRedrawing();

    switch (dir)
    {
        default :
        case 'u' :
            ticksDirection = 0;
            break;
        case 'd' :
            ticksDirection = 1;
            break;
        case 'l' :
            ticksDirection = 2;
            break;
        case 'r' :
            ticksDirection = 3;
            break;
    }

    switch (tics)
    {
        default:
        case 'v':
            ticksStyle = 0;
            break;
        case 'r':
            ticksStyle = 1;
            break;
        case 'i':
            ticksStyle = 2;
            break;
    }

    iObjUID = createAxis(iSubwinUID, ticksDirection, ticksStyle, x, *nx, y, *ny, subint, format, font, textcol, ticscol, seg);

    if (iObjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objdrawaxis");
        return;
    }

    if (val == NULL)
    {
        char **matData;
        StringMatrix *tics_labels;

        tics_labels = computeDefaultTicsLabels(iObjUID);

        if (tics_labels == NULL)
        {
            deleteGraphicObject(iObjUID);
            return;
        }

        matData = getStrMatData(tics_labels);

        /*
         * The labels vector size must be computed using the matrix's dimensions.
         * To be modified when the labels computation is moved to the Model.
         */
        setGraphicObjectProperty(iObjUID, __GO_TICKS_LABELS__, matData, jni_string_vector, tics_labels->nbCol * tics_labels->nbRow);

        deleteMatrix(tics_labels);
    }
    else
    {
        int i = 0;
        /*
         * Labels are set using the str argument; the previous code tested whether each element of the
         * str array was null and set the corresponding Axis' element to NULL, though there was no
         * apparent reason to do so. This is still checked, but now aborts building the Axis.
         */

        if (nb_tics_labels == -1)
        {
            Scierror(999, _("Impossible case when building axis\n"));
            deleteGraphicObject(iObjUID);
            return;
        }

        for (i = 0; i < nb_tics_labels; i++)
        {
            if (val[i] == NULL)
            {
                deleteGraphicObject(iObjUID);
                return;
            }
        }

        setGraphicObjectProperty(iObjUID, __GO_TICKS_LABELS__, val, jni_string_vector, nb_tics_labels);
    }

    setCurrentObject(iObjUID);
}

/*-----------------------------------------------------------
 * Objnumb:
 *-----------------------------------------------------------*/

void Objnumb(char          * fname    ,
             unsigned long   fname_len,
             int         n        ,
             int         flag     ,
             double          x        ,
             double          y        ,
             double        * angle    ,
             double        * box )
{
    /*** faire une macro scilab sur xstring ****/


}

/*------------------------------------------------
 * fec
 *-----------------------------------------------*/
void Objfec (double    x[]        ,
             double    y[]        ,
             double    noeud[]    ,
             double  * fun        ,
             int * n          ,
             int * m          ,
             int * p          ,
             char      strflag[]  ,
             char      legend[]   ,
             double    brect[]    ,
             int   aaint[]    ,
             double    Zminmax[]  ,
             int   Colminmax[],
             int   ColOut[]   ,
             BOOL      WithMesh   ,
             BOOL      flagNax)
{
    C2F(fec)(x, y, noeud, fun, n, m, p, strflag, legend, brect, aaint,
             Zminmax, Colminmax, ColOut, WithMesh, flagNax, 4L, bsiz);
}
/*------------------------------------------------------------------------*/
static int mustUpdate(int iSubwinUID)
{
    int iTmp = 0;
    int * piTmp = &iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_AUTO_SCALE__, jni_bool, (void **)&piTmp);
    return iTmp;
}
/*------------------------------------------------------------------------*/
static void updateXYDataBounds(int iSubwinUID, double rect[6])
{
    int firstPlot = 0;
    int * piFirstPlot = &firstPlot;

    getGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piFirstPlot);
    if (firstPlot)
    {
        rect[4] = 0;
        rect[5] = 0;
    }
    else
    {
        double * dataBounds = NULL;
        getGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

        if (!dataBounds)
        {
            return;
        }
        rect[0] = Min(rect[0], dataBounds[0]);
        rect[1] = Max(rect[1], dataBounds[1]);
        rect[2] = Min(rect[2], dataBounds[2]);
        rect[3] = Max(rect[3], dataBounds[3]);
        rect[4] = dataBounds[4];
        rect[5] = dataBounds[5];
    }

    setGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, rect, jni_double_vector, 6);
}
/*------------------------------------------------------------------------*/
static void updateXYZDataBounds(int iSubwinUID, double rect[6])
{
    int firstPlot = 0;
    int * piFirstPlot = &firstPlot;

    getGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piFirstPlot);
    if (!firstPlot)
    {
        double * dataBounds = NULL;
        getGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

        if (dataBounds)
        {
            rect[0] = Min(rect[0], dataBounds[0]);
            rect[1] = Max(rect[1], dataBounds[1]);
            rect[2] = Min(rect[2], dataBounds[2]);
            rect[3] = Max(rect[3], dataBounds[3]);
            rect[4] = Min(rect[4], dataBounds[4]);
            rect[5] = Max(rect[5], dataBounds[5]);
        }
    }

    setGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, rect, jni_double_vector, 6);
}
/*------------------------------------------------------------------------*/
