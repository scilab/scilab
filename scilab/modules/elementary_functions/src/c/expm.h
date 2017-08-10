/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2012 - Digiteo - Cedric Delamarre
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
#include "elem_common.h"
#include "dynlib_elementary_functions.h"

/*--------------------------------------------------------------------------*/

#if _LCC_ & FORDLL
#define IMPORT_STRUCT __declspec (dllimport)
#else
#ifdef FORDLL
#define IMPORT_STRUCT extern  __declspec (dllimport)
#else
#define IMPORT_STRUCT extern
#endif
#endif

/* csiz used for character coding */
#define csiz 63

/* bsiz size of internal chain buf */
#define bsiz 4096

#ifdef _MSC_VER
/* BUG 3863 */
/* forces to define C2F(dcoeff) only once */
typedef struct
{
    double c[41];
    int ndng;
} DCOEFF_struct;

ELEMENTARY_FUNCTIONS_IMPEXP DCOEFF_struct C2F(dcoeff);

#endif

static double spdblExpmC[41] = {0};
static double sdblExpmN = 0;

/*--------------------------------------------------------------------------*/
void vSwitchVal(double *_pdblVal, int _iPos1, int _iPos2);

void vExchangeVal(  double *_pdblScale, double *_pdblVal,
                    int _iStart1, int _iEnd1,
                    int _iStart2, int _iEnd2,
                    int _iSize, int _iCoord1, int _iCoord2);

int dexpms(int _iLeadDim, int _iSize, double *_pdblVal, double *_pdblReturn);

int dbalancs(   int _iRows, int _iSize, double *_pdblVal,
                int *_piLow, int *_piHigh, double *_pdblScale);

int dbdiaga(int _iLeadDim, int _iSize, double *_pdblVal, double _dblEps,
            double _dblMax, double *_pdblEigenReal, double *_pdblEigenImg,
            int *_piBlockStruc, double *_pdblRightReduce,
            double *_pdblInvRightReduce, double *_pdblScale, int _iMode);

int dorthess(   int _iLead, int _iSize, int _iLow, int _iHigh,
                double *_pdblVal, double *_pdblOrt);

int dortrans(   int _iLead, int _iSize, int _iLow, int _iHigh,
                double *_pdblVal, double *_pdblOrt, double *_pdblTrans);

int dhqror2s(   int _iLead, int _iSize, int _iLow, int _iHigh,
                double *_pdblHessUp, double *_pdblEigenReal,
                double *_pdblEigenImg, double *_pdblTrans, int _iMode);
int dpades( double *_pdblVal, int _iLeadDimIn, int _iSize,
            double *_pdblExp, int _iLeadDimOut, double *_pdblAlpha,
            double *_pdblWS, int *_piWS);

void ddmmuls(   double *_pdblA, int _iLeadDimA,
                double *_pdblB, int _iLeadDimB,
                double *_pdblOut, int _iLeadDimOut,
                int _iRowsA, int _iColsA, int _iColsB);

int dexchs( int _iMax, int _iLeadDim, double *_pdblIn, double *_pdblOut,
            int _iPos, int _iSize1, int _iSize2);

int dgivs(double _dblA, double _dblB, double *_pdblSC, double *_pdblSS);

int dsplits(double *_pdblVal, double *_pdblSplit, int _iSize, int _iPos,
            double *_pdblE1, double *_pdblE2, int _iSizeVal, int _iSizeSplit);

double dblGetMatrixInfiniteNorm(double *_pdblReal, double *_pdblImg, int _iRows, int _iCols);

ELEMENTARY_FUNCTIONS_IMPEXP int dexpms2(double *_pdblReal, double *_pdblReturnReal, int _iLeadDim);

ELEMENTARY_FUNCTIONS_IMPEXP int zexpms2(double *_pdblReal, double *_pdblImg,
                                        double *_pdblReturnReal, double *_pdblReturnImg,
                                        int _iLeadDim);

/*--------------------------------------------------------------------------*/
extern int C2F(split)();
extern int C2F(exch)();
extern int C2F(shrslv)();
extern int C2F(balbak)();
//extern double C2F(ddot)();
extern int C2F(pade)();
//extern int C2F(dcopy)();
//extern int C2F(dscal)();

extern int C2F(dgeco)();
extern int C2F(dgesl)();
extern int C2F(coef)();
extern int C2F(cerr)();
extern int C2F(dclmat)();
extern int C2F(dexpm1)();
extern int C2F(wexpm1)();
extern int C2F(drot)();

