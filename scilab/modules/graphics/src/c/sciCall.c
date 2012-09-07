/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
#include "MALLOC.h" /* MALLOC */
#include "Scierror.h"
#include "sciCall.h"
#include "Plot2d.h"
#include "HandleManagement.h"
#include "stack-def.h" /* bsiz */

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"

/**
 * Put min and max of vector in dMin and dMax.
 * If no min and max can't be found (no finite data in dMin or dMax),
 * then use the default values
 */
static void getDrect(const double vector[], int nbElements,
                     double * dMin, double * dMax,
                     double defaultMin, double defaultMax);
/*------------------------------------------------
 * Objrect :
 * On recupere la figure courante, puis on recupere la sous fenetre qui y est selectionnee
 *   puis on contruit le rectangle, qu on le place comme objet courant
 * ensuite il reste qu'appeler la fonction du dessin de l'objet
 *-----------------------------------------------*/

void Objrect ( double * x         ,
               double * y         ,
               double * width     ,
               double * height    ,
               int    * foreground,
               int    * background,
               BOOL     isfilled  ,
               BOOL     isline    ,
               long   * hdl       )
{
    char* newObjUID = NULL;
    char* psubwinUID = NULL;
    char* pFigureUID = NULL;

    pFigureUID = (char*)getCurrentFigure();
    psubwinUID = (char*)getCurrentSubWin();

    /* check if the auto_clear property is on and then erase everything */
    checkRedrawing();
    /*newObjUID = ConstructRectangle(psubwinUID , *x, *y, *height, *width,
      foreground, background, isfilled, isline);*/

    newObjUID = constructRectangles(psubwinUID, *x, *y, *height, *width,
                                    foreground == NULL ? -1 : *foreground,
                                    background == NULL ? -1 : *background,
                                    (int)isfilled, (int)isline);

    if (newObjUID == NULL)
    {
        /* an error occurred */
        *hdl = -1;
        return;
    }

    setCurrentObject(newObjUID);
    *hdl = getHandle(newObjUID);
}


/*----------------------------------------------
 * Objarc :
 *-----------------------------------------------*/

void Objarc( double * angle1    ,
             double * angle2    ,
             double * x         ,
             double * y         ,
             double * width     ,
             double * height    ,
             int    * foreground,
             int    * background,
             BOOL     isfilled  ,
             BOOL     isline    ,
             long   * hdl        )
{
    char * psubwinUID = NULL;
    char * pobjUID = NULL;

    psubwinUID = (char*)getCurrentSubWin();
    checkRedrawing();
    pobjUID = ConstructArc(psubwinUID, *x, *y,
                           *height, *width, *angle1, *angle2, foreground, background, isfilled, isline);
    setCurrentObject(pobjUID);

    *hdl = getHandle(pobjUID);
    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
}

/*------------------------------------------------
 * Objpoly :
 *-----------------------------------------------*/

void Objpoly ( double  * x     ,
               double  * y     ,
               int   n     ,
               int   closed,
               int       mark  ,
               long    * hdl    )
{
    char * pfigureUID = NULL;
    char * psubwinUID = NULL;
    char * pobjUID = NULL;

    psubwinUID = (char*)getCurrentSubWin();

    checkRedrawing();

    if (mark <= 0)
    {
        int absmark = abs(mark);
        pobjUID = ConstructPolyline(psubwinUID, x, y, PD0, closed, n, 1,
                                    NULL, NULL, &absmark, NULL, NULL, FALSE, FALSE, TRUE, FALSE);
    }
    else
    {
        pobjUID = ConstructPolyline(psubwinUID, x, y, PD0, closed, n, 1,
                                    &mark, NULL, NULL, NULL, NULL, TRUE, FALSE, FALSE, FALSE);
    }

    if (pobjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objpoly");
        return;
    }

    setCurrentObject(pobjUID);
    *hdl = getHandle(pobjUID);
}


/*------------------------------------------------
 * Objfpoly :
 *-----------------------------------------------*/

void Objfpoly ( double  * x    ,
                double  * y    ,
                int   n    ,
                int * style,
                long    * hdl  ,
                int   shading )
{
    char* psubwinUID = NULL;
    char* pobjUID = NULL;

    int fillcolor = 0;
    int contourcolor = 0;
    int *piContourColor = &contourcolor;

    int closed = 1; /* we close the polyline by default */

    psubwinUID = (char*)getOrCreateDefaultSubwin();

    checkRedrawing();

    if (shading == 2)
    {
        /* interpolated shading is "on" */
        pobjUID = ConstructPolyline(psubwinUID, x, y, PD0, closed, n,
                                    1, NULL, style, NULL, NULL, NULL, FALSE, TRUE, FALSE, TRUE);
    }
    else
    {

        /* flat mode is "on" */
        if (*style < 0)
        {
            fillcolor = abs(*style);
            pobjUID = ConstructPolyline(psubwinUID, x, y, PD0, closed, n,
                                        1, NULL, &fillcolor, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE);
        }
        else if (*style == 0)
        {
            getGraphicObjectProperty(psubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piContourColor);
            pobjUID = ConstructPolyline(psubwinUID, x, y, PD0, closed, n,
                                        1, &contourcolor, NULL, NULL, NULL, NULL, TRUE, FALSE, FALSE, FALSE);
        }
        else
        {
            /* *style > 0*/
            fillcolor = *style;
            getGraphicObjectProperty(psubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piContourColor);
            pobjUID = ConstructPolyline(psubwinUID, x, y, PD0, closed, n,
                                        1, &contourcolor, &fillcolor, NULL, NULL, NULL, TRUE, TRUE, FALSE, FALSE);
        }

    }

    if (pobjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objfpoly");
        return;
    }

    setCurrentObject(pobjUID);
    *hdl = getHandle(pobjUID);

    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
}


/*-----------------------------------------------------------
 *   Objsegs :
 *-----------------------------------------------------------*/
void Objsegs ( int * style,
               int   flag ,
               int   n1   ,
               double  * x    ,
               double  * y    ,
               double  * z    ,
               double    arsize )
{
    char *pobjUID = NULL;
    char *psubwinUID = NULL;
    int type = 0, colored = 0;
    double *fx = NULL, *fy = NULL; // No fx or fy
    int typeofchamp = -1; /* no champ here, only segs ; this info is useless */

    checkRedrawing() ;
    psubwinUID = (char*)getCurrentSubWin();

    pobjUID = ConstructSegs(psubwinUID, type,
                            x, y, z, n1, n1, (z == NULL ? 0 : n1), // x, y and z have the same size n1
                            fx, fy, flag, style, arsize, colored, typeofchamp);

    if (pobjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objsegs");
        return;
    }

    setCurrentObject(pobjUID);
}
/*-----------------------------------------------------------
 * Objstring:
 *-----------------------------------------------------------*/

/* box is an OUTPUT re-used inside matdes.c in scixstring */
void Objstring( char            ** fname      ,
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
                sciTextAlignment   alignment   )
{
    char * psubwinUID = NULL;
    char * pobjUID = NULL;
    char * pfigureUID = NULL;

    pfigureUID = (char*)getCurrentFigure();
    psubwinUID = (char*)getCurrentSubWin();

    checkRedrawing();

    pobjUID = ConstructText( psubwinUID   ,
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
                             alignment  );

    if (pobjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objstring");
        return;
    }

    *hdl = getHandle(pobjUID);

    setGraphicObjectProperty(pobjUID, __GO_FONT_ANGLE__, angle, jni_double, 1);
}


/*------------------------------------------------
 *  plot2d
 *-----------------------------------------------*/

void Objplot2d ( int       ptype     ,
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
                 BOOL      flagNax    )
{
    plot2dn(ptype, logflags, x, y, n1, n2, style, strflag, legend, brect, aaint, flagNax, 4L, bsiz);
}

/*------------------------------------------------
 *  grayplot
 *-----------------------------------------------*/
void Objgrayplot ( double    x[]      ,
                   double    y[]      ,
                   double    z[]      ,
                   int * n1       ,
                   int * n2       ,
                   char      strflag[],
                   double    brect[]  ,
                   int   aaint[]  ,
                   BOOL      flagNax   )
{
    C2F(xgray)(x, y, z, n1, n2, strflag, brect, aaint, flagNax, bsiz );
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
                 BOOL      flagNax   )
{
    C2F(xgray1)(z, n1, n2, strflag, brect, aaint, flagNax, bsiz);
}

/*------------------------------------------------
 *  Matplot1
 *-----------------------------------------------*/
void Objmatplot1 ( double    z[],
                   int * n1 ,
                   int * n2 ,
                   double    xrect[] )
{
    C2F(xgray2)(z, n1, n2, xrect);
}

/*------------------------------------------------
 *  plot3d
 *-----------------------------------------------*/
void Objplot3d ( char    * fname ,
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
                 int * n3n    )
/* F.Leray 25.04.05 : warning here legends means "X@Y@Z": it is labels writings!! */
/* legends has not the same meaning than inside plot2dn (there, it is really the legends of the plotted curves)*/
{
    sciTypeOf3D typeof3d;
    int flagcolor = 0;
    long *hdltab = NULL;
    int i = 0;

    char *psubwinUID = NULL;
    char *pobjUID = NULL;
    char *parentFigureUID = NULL;

    double drect[6];
    double rotationAngles[2];
    double* dataBounds = NULL;
    char * loc = NULL;
    char * legx = NULL;
    char * legy = NULL;
    char * legz = NULL;
    char* labelId = NULL;
    /*   char * buff = NULL; */
    int flag_x = 1;
    int flag_y = 1;
    int dimvectx = -1;
    int dimvecty = -1;
    int view = 0;
    int linLogFlag;
    int firstPlot = 0;
    int *piFirstPlot = &firstPlot;

    int box = 0;
    int axisVisible = 0;
    int autoScale = 0;
    int *piAutoScale = &autoScale;

    int isoview = 0;
    int clipState = 0;

    char *pNewSurfaceUID = NULL;


    /* Initialisation drect A.C pour debuggueur */
    drect[0] = 0.0;
    drect[1] = 0.0;
    drect[2] = 0.0;
    drect[3] = 0.0;
    drect[4] = 0.0;
    drect[5] = 0.0;

    /* =================================================
     * Force SubWindow properties according to arguments
     * ================================================= */

    parentFigureUID = (char*)getCurrentFigure();
    psubwinUID = (char*)getCurrentSubWin();

    checkRedrawing();

    /* Force 3D view */
    view = 1;
    setGraphicObjectProperty(psubwinUID, __GO_VIEW__, &view, jni_int, 1);

    if ( legend != NULL )
    {
        int textDimensions[2] = {1, 1};
        /* F.Leray 25.04.05 replace the default labels by the user labels if specified */
        loc = (char *) MALLOC( (strlen(legend) + 1) * sizeof(char));
        if ( loc == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "Objplot3d");
        }

        strcpy(loc, legend);

        /*   legx=strtok_r(loc,"@",&buff); */
        legx = strtok(loc, "@");

        if (legx != NULL)
        {
            getGraphicObjectProperty(psubwinUID, __GO_X_AXIS_LABEL__, jni_string, (void **)&labelId);

            setGraphicObjectProperty(labelId, __GO_TEXT_ARRAY_DIMENSIONS__, textDimensions, jni_int_vector, 2);
            setGraphicObjectProperty(labelId, __GO_TEXT_STRINGS__, &legx, jni_string_vector, textDimensions[0]*textDimensions[1]);
        }

        /*   legy=strtok_r((char *)0,"@",&buff); */
        legy = strtok((char *)NULL, "@"); /* NULL to begin at the last read character */
        if ( legy != NULL )
        {
            getGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_LABEL__, jni_string, (void **)&labelId);

            setGraphicObjectProperty(labelId, __GO_TEXT_ARRAY_DIMENSIONS__, textDimensions, jni_int_vector, 2);
            setGraphicObjectProperty(labelId, __GO_TEXT_STRINGS__, &legy, jni_string_vector, textDimensions[0]*textDimensions[1]);
        }

        /*   legz=strtok_r((char *)0,"@",&buff); */
        legz = strtok((char *)NULL, "@");
        if ( legz != NULL )
        {
            getGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_LABEL__, jni_string, (void **)&labelId);

            setGraphicObjectProperty(labelId, __GO_TEXT_ARRAY_DIMENSIONS__, textDimensions, jni_int_vector, 2);
            setGraphicObjectProperty(labelId, __GO_TEXT_STRINGS__, &legz, jni_string_vector, textDimensions[0]*textDimensions[1]);
        }
    }

    /* Force psubwin->logflags to linear */
    linLogFlag = 0;

    setGraphicObjectProperty(psubwinUID, __GO_X_AXIS_LOG_FLAG__, &linLogFlag, jni_bool, 1);
    setGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_LOG_FLAG__, &linLogFlag, jni_bool, 1);
    setGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_LOG_FLAG__, &linLogFlag, jni_bool, 1);


    getGraphicObjectProperty(psubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piFirstPlot);

    if (firstPlot == 0 && (iflag[2] == 0 || iflag[2] == 1))
    {
        /* Nothing to do: we leave as before */
    }
    else
    {
        int labelVisible;

        if (iflag[2] == 0 || iflag[2] == 1)
        {
            if (firstPlot)
            {
                /* 0: OFF */
                box = 0;

                axisVisible = 0;
                setGraphicObjectProperty(psubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
                setGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
                setGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);

                setGraphicObjectProperty(psubwinUID, __GO_BOX_TYPE__, &box, jni_int, 1);

                labelVisible = 0;
                getGraphicObjectProperty(psubwinUID, __GO_X_AXIS_LABEL__, jni_string, (void **)&labelId);
                setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
                getGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_LABEL__, jni_string, (void **)&labelId);
                setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
                getGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_LABEL__, jni_string, (void **)&labelId);
                setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
            }
            /*else no changes : the axes visible properties are driven by the previous plot */
        }
        else if (iflag[2] == 2)
        {
            /* 2: HIDDEN_AXES */
            box = 2;

            /* for 2d use only (when switching to 2d mode) */
            setGraphicObjectProperty(psubwinUID, __GO_BOX_TYPE__, &box, jni_int, 1);

            axisVisible = 0;
            setGraphicObjectProperty(psubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);

            labelVisible = 0;
            getGraphicObjectProperty(psubwinUID, __GO_X_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
            getGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
            getGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
        }
        else if (iflag[2] == 3)
        {
            /* 1: ON */
            box = 1;

            /* for 2d use only (when switching to 2d mode) */
            setGraphicObjectProperty(psubwinUID, __GO_BOX_TYPE__, &box, jni_int, 1);

            axisVisible = 0;
            setGraphicObjectProperty(psubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);

            labelVisible = 1;
            getGraphicObjectProperty(psubwinUID, __GO_X_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
            getGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
            getGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
        }
        else if (iflag[2] == 4)
        {
            /* 1: ON */
            box = 1;
            setGraphicObjectProperty(psubwinUID, __GO_BOX_TYPE__, &box, jni_int, 1);

            axisVisible = 1;
            setGraphicObjectProperty(psubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);

            labelVisible = 1;
            getGraphicObjectProperty(psubwinUID, __GO_X_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
            getGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
            getGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_LABEL__, jni_string, (void **)&labelId);
            setGraphicObjectProperty(labelId, __GO_VISIBLE__, &labelVisible, jni_bool, 1);
        }
    }

    rotationAngles[0] = *alpha;
    rotationAngles[1] = *theta;

    setGraphicObjectProperty(psubwinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    getGraphicObjectProperty(psubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

    getGraphicObjectProperty(psubwinUID, __GO_AUTO_SCALE__, jni_bool, (void **)&piAutoScale);

    if (autoScale)
    {
        /* compute and merge new specified bounds with data bounds */
        switch (iflag[1])
        {
        case 0:  /* do not change data bounds */
            break;
        case 1 :
        case 3 :
        case 5 :
        case 7 : /* Force data bounds=ebox */
            drect[0] = ebox[0]; /*xmin*/
            drect[2] = ebox[2]; /*ymin*/
            drect[1] = ebox[1]; /*xmax*/
            drect[3] = ebox[3]; /*ymax*/
            drect[4] = ebox[4]; /*zmin*/
            drect[5] = ebox[5]; /*zmax*/
            break;
        case 2 :
        case 4 :
        case 6 :
        case 8:/* Force data bounds to the x and y bounds */
            getDrect(x, (*m1) * (*n1), &drect[0], &drect[1], dataBounds[0], dataBounds[1]);
            getDrect(y, (*m2) * (*n2), &drect[2], &drect[3], dataBounds[2], dataBounds[3]);
            getDrect(z, (*m3) * (*n3), &drect[4], &drect[5], dataBounds[4], dataBounds[5]);
            break;
        }

        /* merge data bounds and drect */
        if (!firstPlot)
        {
            drect[0] = Min(dataBounds[0], drect[0]); /* xmin */
            drect[1] = Max(dataBounds[1], drect[1]); /* xmax */
            drect[2] = Min(dataBounds[2], drect[2]); /* ymin */
            drect[3] = Max(dataBounds[3], drect[3]); /* ymax */
            drect[4] = Min(dataBounds[4], drect[4]); /* zmin */
            drect[5] = Max(dataBounds[5], drect[5]); /* zmax */
        }

        if (iflag[1] != 0)
        {
            setGraphicObjectProperty(psubwinUID, __GO_DATA_BOUNDS__, drect, jni_double_vector, 6);
        }
    }

    if (iflag[1] != 0)
    {
        isoview = ( iflag[1] == 3 || iflag[1] == 4 || iflag[1] == 5 || iflag[1] == 6);

        setGraphicObjectProperty(psubwinUID, __GO_ISOVIEW__, &isoview, jni_bool, 1);
    }

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
    if ( typeof3d != SCI_PARAM3D1 )
    {
        if ( *isfac == 1 )
        {
            /* x is considered as a matrix */
            dimvectx = -1;
        }
        else if ( *m1 == 1 ) /* x is a row vector */
        {
            dimvectx = *n1;
        }
        else if ( *n1 == 1 ) /* x is a column vector */
        {
            dimvectx = *m1;
        }
        else /* x is a matrix */
        {
            dimvectx = -1;
        }

        if ( dimvectx > 1 )
        {
            int monotony = checkMonotony( x, dimvectx );
            if ( monotony == 0 )
            {
                Scierror(999, _("%s: x vector is not monotonous.\n"), "Objplot3d");
                return;
            }

            flag_x = monotony;
        }

        if ( *isfac == 1 )
        {
            /* y is considered as a matrix */
            dimvecty = -1;
        }
        else if ( *m2 == 1 ) /* y is a row vector */
        {
            dimvecty = *n2;
        }
        else if ( *n2 == 1 ) /* y is a column vector */
        {
            dimvecty = *m2;
        }
        else /* y is a matrix */
        {
            dimvecty = -1;
        }

        if ( dimvecty > 1 )
        {
            /* test the monotony on y*/
            int monotony = checkMonotony( y, dimvecty );
            if ( monotony == 0 )
            {
                Scierror(999, _("%s: y vector is not monotonous.\n"), "Objplot3d");
                return;
            }

            flag_y = monotony;
        }

        pNewSurfaceUID = ConstructSurface( psubwinUID, typeof3d,
                                           x, y, z, zcol, *izcol, *m, *n, iflag, ebox, flagcolor,
                                           isfac, m1, n1, m2, n2, m3, n3, m3n, n3n);

        if ( pNewSurfaceUID == NULL )
        {
            Scierror(999, _("%s: No more memory.\n"), "Objplot3d");
            return;
        }

        setCurrentObject( pNewSurfaceUID );

        /* Force clipping, 1: CLIPGRF */
        clipState = 1;
        setGraphicObjectProperty(pNewSurfaceUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

        releaseGraphicObjectProperty(__GO_PARENT__, pNewSurfaceUID, jni_string, 1);
    }
    else
    {
        char* pNewPolylineUID = NULL;
        char* currentSubwinUID = NULL;

        if ((hdltab = MALLOC (*n * sizeof (long))) == NULL)
        {
            Scierror(999, "%s: No more memory.\n", fname);
            return;
        }

        currentSubwinUID = (char*)getCurrentSubWin();

        for (i = 0; i < *n; ++i)
        {
            /* F.Leray Pb here: In fact we do not create a Surface but one or several 3D Polylines
               Pb comes when wanting to access the fields "surface_color" or "flag" for example
               in function sciSet (cf. matdes.c).
               Question 1: Are these properties accessible from a SCI_PARAM3D1 ?
               Question 2: Is "flag" obsolete and replaced by "color_mode"?*/

            if ((*n > 0) && (zcol != (double *)NULL))
            {
                if ((int) zcol[i] > 0)
                {
                    int intzcol = (int) zcol[i];
                    pNewPolylineUID = ConstructPolyline
                        (currentSubwinUID,
                         &(x[*m * i]), &(y[*m * i]), &(z[*m * i]), 0, *m, 1,
                         &intzcol, NULL, NULL, NULL, NULL, TRUE, FALSE, FALSE, FALSE);
                }
                else
                {
                    int intzcol = (int) - zcol[i];
                    pNewPolylineUID = ConstructPolyline
                        (currentSubwinUID,
                         &(x[*m * i]), &(y[*m * i]), &(z[*m * i]), 0, *m, 1,
                         NULL, NULL, &intzcol, NULL, NULL, FALSE, FALSE, TRUE, FALSE);
                }
            }
            else
            {
                /* default case, nothing is given */
                int curcolor = 0;
                int *piCurColor = &curcolor;

                getGraphicObjectProperty(currentSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piCurColor);

                pNewPolylineUID = ConstructPolyline(currentSubwinUID,
                                                    &(x[*m * i]), &(y[*m * i]), &(z[*m * i]), 0, *m, 1,
                                                    &curcolor, NULL, NULL, NULL, NULL, TRUE, FALSE, FALSE, FALSE);
            }

            if (pNewPolylineUID == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                FREE(hdltab);
                return;
            }

            setCurrentObject(pNewPolylineUID);
            setGraphicObjectRelationship(currentSubwinUID, pNewPolylineUID);
            releaseGraphicObjectProperty(__GO_PARENT__, pNewPolylineUID, jni_string, 1);
            pNewPolylineUID = NULL;

            pobjUID = (char*)getCurrentObject();

            /* Force clipping, 1: CLIPGRF */
            clipState = 1;
            setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

            hdltab[i] = getHandle(pobjUID);
        }

        /** construct Compound and make it current object**/
        if ( *n > 1 )
        {
            char* o = ConstructCompound (hdltab, *n);
            setCurrentObject(o);
            releaseGraphicObjectProperty(__GO_PARENT__, o, jni_string, 1);
        }
        FREE(hdltab);
    }

    /* =================================================
     * Redraw Figure
     * ================================================= */

    // subwin has been modified

    firstPlot = 0;
    setGraphicObjectProperty(psubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    FREE(loc);
    loc = NULL;

}
/*-----------------------------------------------------------
 * Objaxis:
 *-----------------------------------------------------------*/

void Objdrawaxis ( char     dir    ,
                   char     tics   ,
                   double * x      ,
                   int    * nx     ,
                   double * y      ,
                   int    * ny     ,
                   char   * val[]  ,
                   int      subint ,
                   char   * format ,
                   int      font   ,
                   int      textcol,
                   int      ticscol,
                   char     flag   ,
                   int      seg    ,
                   int      nb_tics_labels )
{
    char* pobjUID = NULL;
    char* psubwinUID = NULL;

    psubwinUID = (char*)getCurrentSubWin();

    checkRedrawing();

    pobjUID = ConstructAxis(
        psubwinUID,
        dir, tics, x, *nx, y, *ny, val, subint, format, font, textcol, ticscol, flag, seg, nb_tics_labels);

    if (pobjUID == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "Objdrawaxis");
        return;
    }

    setCurrentObject(pobjUID);
    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
}

/*-----------------------------------------------------------
 * Objnumb:
 *-----------------------------------------------------------*/

void Objnumb( char          * fname    ,
              unsigned long   fname_len,
              int         n        ,
              int         flag     ,
              double          x        ,
              double          y        ,
              double        * angle    ,
              double        * box       )
{
    /*** faire une macro scilab sur xstring ****/


}

/*------------------------------------------------
 * fec
 *-----------------------------------------------*/
void Objfec ( double    x[]        ,
              double    y[]        ,
              double    noeud[]    ,
              double  * fun        ,
              int * n          ,
              int * m          ,
              char      strflag[]  ,
              char      legend[]   ,
              double    brect[]    ,
              int   aaint[]    ,
              double    Zminmax[]  ,
              int   Colminmax[],
              int   ColOut[]   ,
              BOOL      WithMesh   ,
              BOOL      flagNax     )
{
    C2F(fec)(x, y, noeud, fun, n, m, strflag, legend, brect, aaint,
             Zminmax, Colminmax, ColOut, WithMesh, flagNax, 4L, bsiz);
}
/*------------------------------------------------------------------------*/
static void getDrect(const double vector[], int nbElements,
                     double * dMin, double * dMax,
                     double defaultMin, double defaultMax)
{
    if (containsOneFiniteElement(vector, nbElements))
    {
        *dMin = Mini(vector, nbElements);
        *dMax = Maxi(vector, nbElements);
    }
    else
    {
        *dMin = defaultMin;
        *dMax = defaultMax;
    }
}
/*------------------------------------------------------------------------*/
