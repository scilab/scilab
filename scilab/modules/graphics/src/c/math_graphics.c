/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
 --------------------------------------------------------------------------*/

#include "math_graphics.h"

/*
 * we use spConfig.h for machine constants
 * XXX : spConfig should be merged and unified
 *       with other machine constant scilab code
 */

#define spINSIDE_SPARSE
#include "../../sparse/includes/spConfig.h"

double Mini(const double vect[], int n)
{
    int i = 0;
    double vmin = LARGEST_REAL;
    for (i = 0 ; i < n ; i++)
    {
        /*    if (isinf(vect[i])== 0 && isnan(vect[i])==0 && vect[i] < vmin)  */
        if (finite(vect[i]) == 1 && vect[i] < vmin)
        {
            vmin = vect[i];
        }
    }

    return vmin;
}

double Maxi(const double vect[], int n)
{
    int i = 0;
    double maxi = - LARGEST_REAL;
    for (i = 0 ; i < n ; i++)
    {
        /* if (isinf(vect[i])== 0 && isnan(vect[i])==0 && vect[i] > maxi) */
        if (finite(vect[i]) == 1 && vect[i] > maxi)
        {
            maxi = vect[i];
        }
    }

    return maxi;
}

void MiniMaxi(const double vect[], int n, double * const min, double * const max)
{
    int i = 0;
    double _min = LARGEST_REAL;
    double _max = -LARGEST_REAL;
    for (; i < n ; i++)
    {
        /*    if ( isinf(vect[i])== 0 && isnan(vect[i])==0 && vect[i] < vmin)  */
        if (finite(vect[i]) == 1)
        {
            if (vect[i] < _min)
            {
                _min = vect[i];
            }
            if (vect[i] > _max)
            {
                _max = vect[i];
            }
        }
    }

    *min = _min;
    *max = _max;
}

/*----------------------------------------------------------------------------*/

/* perform the rotation of point from to point dest  */
void rotate2D(double from[2], double center[2], double angle, double dest[2])
{
    rotate2Dim(from, center, cos(angle), sin(angle), dest);
}

/*----------------------------------------------------------------------------*/
/* perform the rotation of point from to point to. */
/* the angle is directly given with its sine and cosine for speed */
void rotate2Dim(double from[2]   ,
                double center[2] ,
                double cosAngle  ,
                double sinAngle  ,
                double dest[2])
{
    double diff[2];

    /* put the center to (0,0) */
    diff[0] = from[0] - center[0];
    diff[1] = from[1] - center[1];

    /* turn and translate back */
    dest[0] = diff[0] * cosAngle - diff[1] * sinAngle + center[0];
    dest[1] = diff[0] * sinAngle + diff[1] * cosAngle + center[1];
}
/*----------------------------------------------------------------------------*/
/* perform the translation of point from to point to with vector trans */
void translate2D(double from[2], double trans[2], double dest[2])
{
    dest[0] = from[0] + trans[0];
    dest[1] = from[1] + trans[1];
}
/*----------------------------------------------------------------------------*/
void iTranslate2D(int from[2], int trans[2], int dest[2])
{
    dest[0] = from[0] + trans[0];
    dest[1] = from[1] + trans[1];
}
/*----------------------------------------------------------------------------*/
void vectSubstract2D(const double vect1[2], const double vect2[], double res[2])
{
    res[0] = vect1[0] - vect2[0];
    res[1] = vect1[1] - vect2[1];
}
/*----------------------------------------------------------------------------*/
void vectAdd2D(const double v1[2], const double v2[2], double res[2])
{
    res[0] = v1[0] + v2[0];
    res[1] = v1[1] + v2[1];
}
/*----------------------------------------------------------------------------*/
void scalarMult2D(const double v[2], const double scalar, double res[2])
{
    res[0] = scalar * v[0];
    res[1] = scalar * v[1];
}
/*----------------------------------------------------------------------------*/
void normalize2d(double vect[2])
{
    double norm = NORM_2D(vect);
    vect[0] /= norm;
    vect[1] /= norm;
}
/*----------------------------------------------------------------------------*/
void iNormalize2d(int vect[2])
{
    double norm = NORM_2D(vect);
    vect[0] = scilab_round(vect[0] / norm);
    vect[1] = scilab_round(vect[1] / norm);
}
/*----------------------------------------------------------------------------*/
BOOL isPointInTriangle(const double point[2], const double a[2],
                       const double b[2], const double c[2])
{
    return (   areOnSameSideOfLine(point, a, b, c)
               && areOnSameSideOfLine(point, b, a, c)
               && areOnSameSideOfLine(point, c, a, b));
}
/*----------------------------------------------------------------------------*/
BOOL areOnSameSideOfLine(const double p1[2], const double p2[2],
                         const double a[2], const double b[2])
{
    // point are on the same if and only if (AB^AP1).(AB^AP2) >= 0
    double ab[3];
    double ap1[3];
    double ap2[3];
    double cp1[3];
    double cp2[3];

    ab[0] = b[0] - a[0];
    ab[1] = b[1] - a[1];
    ab[2] = 0.0;

    ap1[0] = p1[0] - a[0];
    ap1[1] = p1[1] - a[1];
    ap1[2] = 0.0;

    ap2[0] = p2[0] - a[0];
    ap2[1] = p2[1] - a[1];
    ap2[2] = 0.0;

    crossProduct(ab, ap1, cp1);
    crossProduct(ab, ap2, cp2);

    return (DOT_PROD_3D(cp1, cp2) >= 0.0);

}
/*----------------------------------------------------------------------------*/
void crossProduct(const double v1[3], const double v2[3], double res[3])
{
    /* save data to be able to use v1 o v2 as res */
    double v10 = v1[0];
    double v20 = v2[0];
    double v11 = v1[1];
    double v21 = v2[1];
    res[0] = v11 * v2[2] - v1[2] * v21 ;
    res[1] = v1[2] * v20 - v10 * v2[2] ;
    res[2] = v10 * v21 - v11 * v20 ;
}
/*----------------------------------------------------------------------------*/
void vectSubstract3D(const double v1[3] , const double v2[3], double res[3])
{
    res[0] = v1[0] - v2[0];
    res[1] = v1[1] - v2[1];
    res[2] = v1[2] - v2[2];
}
/*----------------------------------------------------------------------------*/
void vectAdd3D(const double v1[3], const double v2[3], double res[3])
{
    res[0] = v1[0] + v2[0];
    res[1] = v1[1] + v2[1];
    res[2] = v1[2] + v2[2];
}
/*----------------------------------------------------------------------------*/
void scalarMult3D(const double v[3], double scalar, double res[3])
{
    res[0] = scalar * v[0];
    res[1] = scalar * v[1];
    res[2] = scalar * v[2];
}
/*----------------------------------------------------------------------------*/
void normalize3D(double vect[3])
{
    double norm = NORM_3D(vect) ;
    vect[0] /= norm ;
    vect[1] /= norm ;
    vect[2] /= norm ;
}
/*----------------------------------------------------------------------------*/
void setToIdentity(double mat4D[4][4])
{
    int i = 0;
    int j = 0;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            mat4D[i][j] = 0.0;
        }
        mat4D[i][i] = 1.0;
    }
}
/*----------------------------------------------------------------------------*/
void mat4DMult(const double mat4D[4][4], const double vect3D[3], double res[3])
{
    int i = 0;
    double res4D[4];
    // w coordinate of the vector is supposed to be 1;
    for (i = 0; i < 4; i++)
    {
        res4D[i]  =  vect3D[0] * mat4D[i][0] + vect3D[1] * mat4D[i][1]
                     + vect3D[2] * mat4D[i][2] + mat4D[i][3];
    }
    res[0] = res4D[0] / res4D[3];
    res[1] = res4D[1] / res4D[3];
    res[2] = res4D[2] / res4D[3];
}
/*----------------------------------------------------------------------------*/
