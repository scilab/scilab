/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to BUILD new objects :
      - allocating memory
      - setting default value
      - binding the newly created object tyo the entire existing hierarchy
 --------------------------------------------------------------------------*/

#ifndef __SCI_BUILD__
#define __SCI_BUILD__

#include "dynlib_graphics.h"
#include "BOOL.h"

/** @name Text Alignment
 * Text alignement enum.
 */
typedef enum
{
    ALIGN_NONE = 0,
    ALIGN_LEFT = 1,
    ALIGN_CENTER = 2,
    ALIGN_RIGHT = 3
}
sciTextAlignment;

/** @name Type of 3D Object
 * Type of 3D Object enum
 */
typedef enum
{
    SCI_FAC3D = 0,
    SCI_PLOT3D = 1,
    SCI_CONTOUR,
    SCI_PARAM3D,
    SCI_PARAM3D1
}
sciTypeOf3D;

/**
 * If a current figure exists : return it
 * Otherwise create a new one.
 */
GRAPHICS_IMPEXP int getOrCreateDefaultSubwin(void);
GRAPHICS_IMPEXP int ConstructText(int iParentsubwinUID, char **text, int nbRow, int nbCol, double x,
                                  double y, BOOL autoSize, double userSize[2], BOOL centerPos, int *foreground, int *background,
                                  BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align);
GRAPHICS_IMPEXP int ConstructLegend(int iParentsubwinUID, char *text[], int* tabofhandles, int nblegends);   /* BUILD */
GRAPHICS_IMPEXP int ConstructPolyline(int iParentsubwinUID, double *pvecx, double *pvecy, double *pvecz, int closed, int n1, int plot, int *foreground, int *background, int *mark_style, int *mark_foreground, int *mark_background, BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded);    /* BUILD */
GRAPHICS_IMPEXP int ConstructSurface(int iParentsubwinUID, sciTypeOf3D typeof3d, double *pvecx, double *pvecy, double *pvecz, double *zcol, int izcol, int dimzx, int dimzy, int *flag, double *ebox, int flagcolor, int *isfac, int *m1, int *n1, int *m2, int *n2, int *m3, int *n3, int *m3n, int *n3n); /* BUILD */
GRAPHICS_IMPEXP int ConstructGrayplot(int iParentsubwinUID, double *vx, double *vy, double *vz, int nx, int ny, int type);  /* BUILD */
GRAPHICS_IMPEXP int ConstructImplot(int iParentsubwinUID, double * rect, unsigned char *vz, int nx, int ny, int plottype);  /* BUILD */
GRAPHICS_IMPEXP int ConstructFec(int iParentsubwinUID, double *pvecx, double *pvecy, double *pnoeud, double *pfun, int Nnode, int Ntr, int Nvertex, double *zminmax, int *colminmax, int *colout, BOOL with_mesh);   /* BUILD */
GRAPHICS_IMPEXP int ConstructLight(char* fname, int iSubwin, int type, BOOL visible, double * position, double * direction, double * ambient_color, double * diffuse_color, double * specular_color);

#endif /* __SCI_BUILD__ */
