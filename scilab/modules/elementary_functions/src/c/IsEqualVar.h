/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Serge STEER
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

/* --------------------------------------------------------------------------*/

#ifndef __ISEQUALVAR__
#define __ISEQUALVAR__

#include <string.h>
#include "sci_malloc.h"

static int IsEqualDoubleMat(double *d1, double *d2);
static int IsEqualPolyMat(double *d1, double *d2);
static int IsEqualBoolMat(double *d1, double *d2);
static int IsEqualDoubleSparseMat(double *d1, double *d2);
static int IsEqualBoolSparseMat(double *d1, double *d2);
static int IsEqualMatlabSparseMat(double *d1, double *d2);
static int IsEqualIntegerMat(double *d1, double *d2);
static int IsEqualStringMat(double *d1, double *d2);
static int IsEqualLib(double *d1, double *d2);
static int IsEqualList(double *d1, double *d2);
static int IsEqualLUPtr(double *d1, double *d2);
static int IsEqualOverloaded(double *d1, int n1, double *d2, int n2);
static int IsEqualDoubleArray(int n, double *d1, double *d2);
static int IsEqualDoubleArrayBinary(int n,  double  *d1, double *d2);
static int IsEqualDoubleArrayIEEE(int n, double *d1, double *d2);
static int IsEqualIntegerArray(int n, int *d1, int *d2);
static int IsEqualShortIntegerArray(int typ, int n, int *d1, int *d2);
static int IsEqualFunction(double *d1, double *d2);
static int IsEqualVar(double *d1, int n1, double *d2, int n2);

/* comparison mode for double precision numbers */
void SetDoubleCompMode(int mode);
int GetDoubleCompMode(void);
int DoubleCompMode = 1; /*IEEE mode */

/* Structure for walking inside Scilab lists (used by IsEqualList)*/
int AllocRecIfRequired(int krec);
void FreeRec(void);
typedef struct RecursionRecord
{
    double* d1 ;/* pointers on the first list header */
    double* d2 ;/* pointers on the second list header */
    int     k; /* index of current list element  */
} RecursionRecord, *RecursionRecordPtr;

RecursionRecordPtr Rrec;
int MaxRec; /* allocated size for the array Rrec, 0 means not allocated */

/* macros for debugging */
/*#define DEBUG_BASE(fmt, ...)sciprint(fmt, __VA_ARGS__) */;
/*#define DEBUG_LIST(fmt, ...) sciprint(fmt, __VA_ARGS__) */ ;
/*#define DEBUG_OVERLOADING(fmt, ...) sciprint(fmt, __VA_ARGS__) */ ;


/**intisequalvar
 * Gateway for isequalbitwise and isequal builtins
 * @param char * fname: the Scilab code of the function name
 * @param int * job: if *job==0 the floating point numbers are compared bitwize ,
 *      if *job==1 the comparison is made numerically,
 *      so NaN elements are not equal
 *      elements of with differents data types are raised to the upper types before comparison (to be done)
 * @return 0 in any cases
 * @author Serge Steer
 * @see isequal
 */
int C2F(intisequalvar)(char * fname, int *job, long int fl); /* the gateway */


#endif /* !__ISEQUALVAR__ */
