/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2000 - ENPC - Jean-Philipe Chancelier
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

#ifndef __SCIMATH_H__
#define __SCIMATH_H__

#include <stdlib.h>
#include "dynlib_graphics.h"
#include "BOOL.h"
#include "core_math.h"

#define PI0 (int *) 0
#define PD0 (double *) 0
#define SMDOUBLE 1.e-200 /* Smalest number to avoid dividing by zero */

#if (defined(sun) || defined(SYSV))
#include <ieeefp.h>
#endif

#if defined(_MSC_VER)
#define M_PI 3.14159265358979323846
#else
#if defined(HAVE_VALUES_H)
#include <values.h>
#else
#if defined(HAVE_LIMITS_H)
#include <limits.h>
#endif
#endif
#endif

GRAPHICS_IMPEXP double Mini(const double vect[], int n);

GRAPHICS_IMPEXP double Maxi(const double vect[], int n);

GRAPHICS_IMPEXP void MiniMaxi(const double vect[], int n, double * const min, double * const max);

/* prototypes */

/*----------------------------------------------------------------------------*/
/************************************************************************/
/* 2D algorithms                                                        */
/************************************************************************/
/* perform the rotation of point from to point dest with angle in radian  */
GRAPHICS_IMPEXP void rotate2D(double from[2], double center[2], double angle, double dest[2]);

/* perform the rotation of point from to point to. */
/* the angle is directly given with its sine and cosine for speed */
GRAPHICS_IMPEXP void rotate2Dim(double from[2]   ,
                                double center[2] ,
                                double cosAngle  ,
                                double sinAngle  ,
                                double dest[2]);

/* perform the translation of point from to point dest with vector trans */
GRAPHICS_IMPEXP void translate2D(double from[2], double trans[2], double dest[2]);

/* perform the translation of point from to point dest with vector trans given in pixels.*/
GRAPHICS_IMPEXP void iTranslate2D(int from[2], int trans[2], int dest[2]);

/**
 * Normalize a 2d vector.
 */
GRAPHICS_IMPEXP void normalize2d(double vect[2]);

/**
 * Normalize a 2d vector in pixels coordinates.
 */
GRAPHICS_IMPEXP void iNormalize2d(int vect[2]);

/**
 * Perform the subtraction of vect1 and vect2 and store it in res
 */
GRAPHICS_IMPEXP void vectSubstract2D(const double vect1[2], const double vect2[], double res[2]);

/**
 * subtraction of two vector
 * @param res result of v1 + v2
 */
GRAPHICS_IMPEXP void vectAdd2D(const double v1[2], const double v2[2], double res[2]);

/**
 * Multiply a vector by a scalar
 * @param res scalar.v
 */
GRAPHICS_IMPEXP void scalarMult2D(const double v[2], const double scalar, double res[2]);

/**
 * return the scalar product of two 2d vectors.
 */
#define DOT_PROD_2D(v1,v2) (((v1)[0]) * ((v2)[0]) + ((v1)[1]) * ((v2)[1]))

/**
 * Square norm of a vector
 */
#define SQUARE_NORM_2D(v) (DOT_PROD_2D((v),(v)))

/**
 * Norm of a 2D vector
 */
#define NORM_2D(v) (sqrt(SQUARE_NORM_2D(v)))

/**
 * Compute whether the point lies within the triangle defined by A, B and C
 */
GRAPHICS_IMPEXP BOOL isPointInTriangle(const double point[2], const double a[2],
                                       const double b[2], const double c[2]);

/**
 * Compute whether p1 and p2 are on the same side of line (A,B).
 */
GRAPHICS_IMPEXP BOOL areOnSameSideOfLine(const double p1[2], const double p2[2],
        const double a[2], const double b[2]);

/************************************************************************/
/* 3D algorithms                                                        */
/************************************************************************/

/**
 * Scalar product of 2 vectors in 3D
 */
#define DOT_PROD_3D(v1,v2) (((v1)[0]) * ((v2)[0]) + ((v1)[1]) * ((v2)[1]) + ((v1)[2]) * ((v2)[2]))

/**
 * Cross product of two vector
 * @param res v1^v2
 */
GRAPHICS_IMPEXP void crossProduct(const double v1[3], const double v2[3], double res[3]);

/**
 * Square norm of a 3D vector
 */
#define SQUARE_NORM_3D(v) (DOT_PROD_3D((v),(v)))

/**
 * Norm of a 3D vector
 */
#define NORM_3D(v) (sqrt(SQUARE_NORM_3D(v)))

/**
 * subtraction of two vector
 * @param res result of v1 - v2
 */
GRAPHICS_IMPEXP void vectSubstract3D(const double v1[3], const double v2[3], double res[3]);

/**
 * subtraction of two vector
 * @param res result of v1 + v2
 */
GRAPHICS_IMPEXP void vectAdd3D(const double v1[3], const double v2[3], double res[3]);

/**
 * Multiply a vector by a scalar
 * @param res scalar.v
 */
GRAPHICS_IMPEXP void scalarMult3D(const double v[3], double scalar, double res[3]);

/**
 * Normalize a 3D vector
 */
GRAPHICS_IMPEXP void normalize3D(double vect[3]);

/**
 * Set a 4x4 matrix to identity.
 */
GRAPHICS_IMPEXP void setToIdentity(double mat4D[4][4]);

/**
 * Compute the product mat4D*vect3D and store the result in res.
 */
GRAPHICS_IMPEXP void mat4DMult(const double mat4D[4][4], const double vect3D[3], double res[3]);

/*----------------------------------------------------------------------------*/

/* check if two values can be considered equal given an accurracy */
/* the x == y test is put to avoid division by 0 */
#define SAFE_EQUAL2(x,y,acc) (Abs((x) - (y)) / Max(Abs((x) + (y)), EPSILON) < (acc))

#define SAFE_EQUAL(x,y,acc) ((x) == (y)) || (Abs((x) - (y)) < (acc) * Max(Abs((x)), Abs((y))))
/* test if a value if lesser than an other or just sligthy greater */
#define SAFE_LT(x,y,acc) ((x) == (y) || ((x) - (y)) < (acc) * Max(Abs((x)), Abs((y))))
#define SAFE_GT(x,y,acc) ((x) == (y) || ((y) - (x)) < (acc) * Max(Abs((x)), Abs((y))))
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* truncate an int to a 16 bits integer. Overflow value are truncated to the min */
/* and max of 16 bits integer. */
#define INT_2_16B(i) ((i) > INT16MAX ? (INT16MAX) : ((i) < INT16MIN ? INT16MIN : ((SCIINT16) i)))
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* truncate an int to a char. Overflow value are truncated to the min */
/* and max of char integer. */
#define INT_2_UCHAR(i) ((i) > UCHAR_MAX ? (UCHAR_MAX) : ((i) < 0  ? 0 : ((char) i)))
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* truncate a float or double to an int. Overflow value are truncated to the min */
/* and max of 32 bits integer. */
#define FLOAT_2_INT(f) ((f) > INT_MAX ? (INT_MAX) : ((f) < INT_MIN ? (INT_MIN) : (inint(f))))
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return the number of digits of a positive int (ie for 10236 it is 5)  */
/* the added 0.5 is to avoid trouble with 10^i which could become 9.999999999 */
#define GET_NB_DIGITS_POS(i) ((i) == 0 ? 1 : ((int) floor(log10((i) + 0.5))) + 1)
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return the number of digits of an int (ie for 10236 it is 5 or -102 it is 4)  */
/* For negative values, the length is increased by 1 because of the '-' sign" */
#define GET_NB_DIGITS(i) ((i) > 0 ? GET_NB_DIGITS_POS(i) : GET_NB_DIGITS_POS(-i) + 1)
/*----------------------------------------------------------------------------*/




#endif /* SCI_MATH.HG */

