/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "matrix_power.h"
#include "matrix_multiplication.h"
//#include "unsfdcopy.h"

#include "elementary_functions.h"
#include "elem_common.h"
#include "invert_matrix.h"
#include "sciprint.h"
#include "localization.h"
#include "configvariable_interface.h"
#include "Sciwarning.h"

/*
r : real part
c : imaginary part
E : element of
R : real
Z : integer
C : complex
* : less 0
+ : >= 0
- : <= 0
*/

/*ddpowe*/
int iPowerRealScalarByRealScalar(
    double _dblReal1,
    double _dblReal2,
    double *_pdblRealOut, double *_pdblImgOut, int *_piComplex)
{
    //exposant is an integer
    if ((int)_dblReal2 == _dblReal2)
    {
        //dipowe
        int iReal2 = (int)_dblReal2;

        if (iReal2 == 1)
        {
            //R ^ 1
            *_pdblRealOut = _dblReal1;
            *_pdblImgOut	= 0;
            *_piComplex		= 0;
        }
        else if (iReal2 == 0)
        {
            //R ^ 0
            *_pdblRealOut = 1;
            *_pdblImgOut	= 0;
            *_piComplex		= 0;
        }
        else if (iReal2 < 0)
        {
            //R ^ Z-
            if (_dblReal1 != 0)
            {
                //R* ^ Z-
                *_pdblRealOut = pow(_dblReal1, iReal2);
                *_pdblImgOut	= 0;
                *_piComplex		= 0;
            }
            else
            {
                //0 ^ 0
                //FIXME : ieee
                //generate +Inf
                double dblZero	= 0.0;
                *_pdblRealOut		= 1.0 / (dblZero);
                *_pdblImgOut		= 0;
                *_piComplex			= 0;
            }
        }
        else
        {
            //R ^ Z*+ ( N* )
            *_pdblRealOut = pow(_dblReal1, iReal2);
            *_pdblImgOut	= 0;
            *_piComplex		= 0;
        }
    }
    else
    {
        if (_dblReal1 > 0)
        {
            //R*+ ^ R
            *_pdblRealOut = pow(_dblReal1, _dblReal2);
            *_pdblImgOut	= 0;
            *_piComplex		= 0;
        }
        else if (_dblReal1 < 0)
        {
            //R*- ^ R
            double dblRealTemp	= 0;
            double dblImgTemp		= 0;

            wlog(_dblReal1, 0, &dblRealTemp, &dblImgTemp);
            dblRealTemp					= dexps(dblRealTemp * _dblReal2);
            dblImgTemp					= dblImgTemp * _dblReal2;
            *_pdblRealOut				= dblRealTemp * dcoss(dblImgTemp);
            *_pdblImgOut				= dblRealTemp * dsins(dblImgTemp);
            *_piComplex					= 1;
        }
        else if (_dblReal1 == 0)
        {
            //0 ^ R
            if (_dblReal2 < 0)
            {
                //0 ^ R*-
                //FIXME : ieee
                //generate +Inf
                double dblZero	= 0.0;
                *_pdblRealOut		= 1.0 / (dblZero);
                *_pdblImgOut		= 0;
                *_piComplex			= 0;
            }
            else if (_dblReal2 == 0)
            {
                //0 ^ 0
                //never call, pass in R ** 0 before
                *_pdblRealOut		= 1;
                *_pdblImgOut		= 0;
                *_piComplex			= 0;
            }
            else if (_dblReal2 > 0)
            {
                //0 ^ R*+
                *_pdblRealOut		= 0;
                *_pdblImgOut		= 0;
                *_piComplex			= 0;
            }
            else //_dblReal2 is NaN
            {
                *_pdblRealOut		= _dblReal2;
                *_pdblImgOut		= 0;
                *_piComplex			= 0;
            }
        }
        else //_dblReal1 is NaN
        {
            *_pdblRealOut		= _dblReal1;
            *_pdblImgOut		= 0;
            *_piComplex			= 0;
        }
    }
    return 0;
}

/*dwpowe*/
int iPowerRealScalarByComplexScalar(
    double _dblReal1,
    double _dblReal2, double _dblImg2,
    double* _pdblRealOut, double* _pdblImgOut)
{
    if (_dblImg2 == 0)
    {
        //R ^ R
        int iComplex = 0;
        iPowerRealScalarByRealScalar(
            _dblReal1,
            _dblReal2,
            _pdblRealOut, _pdblImgOut, &iComplex);
    }
    else
    {
        //R ^ C
        if (_dblReal1 != 0)
        {
            //R* ^ C
            double dblRealTemp	= 0;
            double dblImgTemp		= 0;

            wlog(_dblReal1, 0, &dblRealTemp, &dblImgTemp);
            C2F(wmul)(&dblRealTemp, &dblImgTemp, &_dblReal2, &_dblImg2, &dblRealTemp, &dblImgTemp);
            dblRealTemp					= dexps(dblRealTemp);
            *_pdblRealOut				= dblRealTemp * dcoss(dblImgTemp);
            *_pdblImgOut				= dblRealTemp * dsins(dblImgTemp);
        }
        else
        {
            //0 ^ C
            if (_dblReal2 > 0)
            {
                //0 ^ (r E R*+ ) & ( c E R )
                *_pdblRealOut				= 0;
                *_pdblImgOut				= 0;
            }
            else if (_dblReal2 < 0)
            {
                //0 ^ (r E R*- ) & ( c E R )
                //FIXME : ieee
                //generate +Inf
                double dblZero	= 0.0;
                *_pdblRealOut		= 1.0 / (dblZero);
                *_pdblImgOut		= 0;
            }
            else //_dblReal2 == 0, NaN
            {
                //0 ^ (r = 0 ) & ( c E R )
                *_pdblRealOut				= 1;
                *_pdblImgOut				= 0;
            }
        }
    }
    return 0;
}

/*wdpowe*/
int iPowerComplexScalarByRealScalar(
    double _dblReal1, double _dblImg1,
    double _dblReal2,
    double* _pdblRealOut, double* _pdblImgOut)
{
    if ((int)_dblReal2 == _dblReal2)
    {
        //C ^ Z
        if (_dblReal2 == 0)
        {
            //C ^ 0
            *_pdblRealOut				= 1;
            *_pdblImgOut				= 0;
        }
        else if (_dblReal2 < 0)
        {
            //C ^ Z*-
            if (dabss(_dblReal1) + dabss(_dblImg1) != 0) //_dblReal1 != 0 || _dblImg1 != 0 ?
            {
                int i = 0;
                double dblOne				= 1;
                double dblZero			= 0;
                double dblRealTemp	= 0;
                double dblImgTemp		= 0;

                C2F(wdiv)(&dblOne, &dblZero, &_dblReal1, &_dblImg1, _pdblRealOut, _pdblImgOut);

                dblRealTemp					= *_pdblRealOut;
                dblImgTemp					= *_pdblImgOut;

                for (i = 2 ; i <= dabss(_dblReal2) ; i++)
                {
                    C2F(wmul)(&dblRealTemp, &dblImgTemp, _pdblRealOut, _pdblImgOut, _pdblRealOut, _pdblImgOut);
                }
            }
            else
            {
                //FIXME : ieee
                //generate +Inf
                double dblZero	= 0.0;
                *_pdblRealOut		= 1.0 / (dblZero);
                *_pdblImgOut		= 0;
            }
        }
        else
        {
            //C ^ Z*+
            int i								= 0;
            double dblRealTemp	= 0;
            double dblImgTemp		= 0;

            *_pdblRealOut			= _dblReal1;
            *_pdblImgOut			= _dblImg1;
            dblRealTemp				= *_pdblRealOut;
            dblImgTemp				= *_pdblImgOut;

            for (i = 2 ; i <= dabss(_dblReal2) ; i++)
            {
                C2F(wmul)(&dblRealTemp, &dblImgTemp, _pdblRealOut, _pdblImgOut, _pdblRealOut, _pdblImgOut);
            }
        }
    }
    else
    {
        if (dabss(_dblReal1) + dabss(_dblImg1) != 0)
        {
            //C ^ R
            double dblRealTemp	= 0;
            double dblImgTemp		= 0;

            wlog(_dblReal1, _dblImg1, &dblRealTemp, &dblImgTemp);
            dblRealTemp					= dexps(dblRealTemp * _dblReal2);
            dblImgTemp					= dblImgTemp * _dblReal2;
            *_pdblRealOut				= dblRealTemp * dcoss(dblImgTemp);
            *_pdblImgOut				= dblRealTemp * dsins(dblImgTemp);
        }
        else
        {
            if (_dblReal2 > 0)
            {
                //0 ^ R*+
                *_pdblRealOut		= 0;
                *_pdblImgOut		= 0;
            }
            else if (_dblReal2 < 0)
            {
                //0 ^ R*-
                //FIXME : ieee
                //generate +Inf
                double dblZero	= 0.0;
                *_pdblRealOut		= 1.0 / (dblZero);
                *_pdblImgOut		= 0;
            }
            else
            {
                //0 ^ 0
                *_pdblRealOut		= 1;
                *_pdblImgOut		= 0;
            }
        }
    }
    return 0;
}

/*wwpowe*/
int iPowerComplexScalarByComplexScalar(
    double _dblReal1, double _dblImg1,
    double _dblReal2, double _dblImg2,
    double* _pdblRealOut, double* _pdblImgOut)
{
    if (_dblImg2 == 0)
    {
        //C ^ R
        iPowerComplexScalarByRealScalar(
            _dblReal1, _dblImg1,
            _dblReal2,
            _pdblRealOut, _pdblImgOut);
    }
    else
    {
        //C ^ C
        if (dabss(_dblReal1) + dabss(_dblImg1) != 0)
        {
            // ! 0 ^ C
            double dblRealTemp = 0;
            double dblImgTemp  = 0;

            wlog(_dblReal1, _dblImg1, &dblRealTemp, &dblImgTemp);
            C2F(wmul)(&dblRealTemp, &dblImgTemp, &_dblReal2, &_dblImg2, &dblRealTemp, &dblImgTemp);
            dblRealTemp   = dexps(dblRealTemp);
            *_pdblRealOut = dblRealTemp * dcoss(dblImgTemp);
            *_pdblImgOut  = dblRealTemp * dsins(dblImgTemp);
        }
        else
        {
            // 0 ^ C
            //FIXME : ieee
            //generate +Inf
            double dblZero = 0.0;
            *_pdblRealOut  = 1.0 / (dblZero);
            *_pdblImgOut   = 0;
        }
    }
    return 0;
}

/*s .^ []*/
int iPowerRealScalarByRealMatrix(
    double _dblReal1,
    double* _pdblReal2, int _iRows2, int _iCols2,
    double* _pdblRealOut,	double* _pdblImgOut, int *_piComplex)
{
    int i = 0;
    *_piComplex = 0;
    for (i = 0 ; i < _iRows2 * _iCols2 ; i++)
    {
        int iComplex = 0;
        iPowerRealScalarByRealScalar(
            _dblReal1, _pdblReal2[i], &_pdblRealOut[i], &_pdblImgOut[i], &iComplex);
        *_piComplex |= iComplex;
    }
    return 0;
}


int iPowerComplexScalarByRealMatrix(
    double _dblReal1, double _dblImg1,
    double* _pdblReal2, int _iRows2, int _iCols2,
    double* _pdblRealOut,	double* _pdblImgOut)
{
    int i = 0;
    for (i = 0 ; i < _iRows2 * _iCols2 ; i++)
    {
        iPowerComplexScalarByRealScalar(
            _dblReal1, _dblImg1, _pdblReal2[i], &_pdblRealOut[i], &_pdblImgOut[i]);
    }
    return 0;
}

int iPowerRealScalarByComplexMatrix(
    double _dblReal1,
    double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
    double* _pdblRealOut,	double* _pdblImgOut)
{
    int i = 0;
    for (i = 0 ; i < _iRows2 * _iCols2 ; i++)
    {
        iPowerRealScalarByComplexScalar(
            _dblReal1, _pdblReal2[i], _pdblImg2[i], &_pdblRealOut[i], &_pdblImgOut[i]);
    }
    return 0;
}

int iPowerComplexScalarByComplexMatrix(
    double _dblReal1, double _dblImg1,
    double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
    double* _pdblRealOut,	double* _pdblImgOut)
{
    int i = 0;
    for (i = 0 ; i < _iRows2 * _iCols2 ; i++)
    {
        iPowerComplexScalarByComplexScalar(
            _dblReal1, _dblImg1, _pdblReal2[i], _pdblImg2[i], &_pdblRealOut[i], &_pdblImgOut[i]);
    }
    return 0;
}

//Square Matrix ^ Scalar
int iPowerRealSquareMatrixByRealScalar(
    double* _pdblReal1, int _iRows1, int _iCols1,
    double _dblReal2,
    double* _pdblRealOut,	double* _pdblImgOut, int *_iComplex)
{
    int iInv = 0;
    int iExpRef = (int)_dblReal2;
    if (iExpRef < 0)
    {
        //call matrix invetion
        iInv = 1;
        iExpRef = -iExpRef;
    }

    if ((int)_dblReal2 == _dblReal2) //integer exponent
    {
        if (iExpRef == 1)
        {
            int iSize = _iRows1 * _iCols1;
            int iOne = 1;
            C2F(dcopy)(&iSize, _pdblReal1, &iOne, _pdblRealOut, &iOne);
        }
        else if (iExpRef == 0)
        {
            int iSize       = _iRows1 * _iCols1;
            int iOne        = 1;
            double dblOne   = 1;
            double dblZero  = 0;
            int iRowp1      = _iRows1 + 1;

            if (C2F(dasum)(&iSize, _pdblReal1, &iOne) == 0)
            {
                //Invalid exponent
                return 1;
            }
            C2F(dset)(&iSize, &dblZero, _pdblRealOut, &iOne);
            C2F(dset)(&_iRows1, &dblOne, _pdblRealOut, &iRowp1);
        }
        else
        {
            int i           = 0;
            int iOne        = 1;
            int iPos        = 0;
            int iPrevPos    = 0;
            int iSize       = _iCols1 * _iCols1;
            int iInc        = _iCols1 + 1;

            double alpha = 1.;
            double beta  = 0.;

            double* pdblResult  = (double*)malloc(iSize * sizeof(double));
            double* pdblTmp     = (double*)malloc(iSize * sizeof(double));

            // initialize the output as identity matrix
            // because we use this array in the first multiplication.
            memset(_pdblRealOut, 0x00, iSize * sizeof(double));
            C2F(dset)(&_iCols1, &alpha, _pdblRealOut, &iInc);

            // copy input data to avoid input modification
            C2F(dcopy)(&iSize, _pdblReal1, &iOne, pdblTmp, &iOne);

            // find all "1" in binary representation of the power integer.
            // then perform multiplication according the "1" position.
            while (iExpRef != 0)
            {
                if (iExpRef & 1u)
                {
                    // start the loop at the last position
                    // because we keep the last result to perform
                    // the multiplication.
                    for (i = iPrevPos; i < iPos; i++)
                    {
                        double* pdblSwitch;
                        C2F(dgemm)( "N", "N", &_iCols1, &_iCols1, &_iCols1, &alpha, pdblTmp, &_iCols1,
                                    pdblTmp, &_iCols1, &beta, pdblResult, &_iCols1);

                        pdblSwitch  = pdblTmp;
                        pdblTmp     = pdblResult;
                        pdblResult  = pdblSwitch;
                    }

                    iPrevPos = iPos;

                    C2F(dgemm)( "N", "N", &_iCols1, &_iCols1, &_iCols1, &alpha, pdblTmp, &_iCols1,
                                _pdblRealOut, &_iCols1, &beta, pdblResult, &_iCols1);

                    C2F(dcopy)(&iSize, pdblResult, &iOne, _pdblRealOut, &iOne);
                }

                iPos++;
                iExpRef = iExpRef >> 1;
            }

            free(pdblResult);
            free(pdblTmp);
        }
    }
    else
    {
        //floating point exponent
        return -1; // manage by overload
    }

    if (iInv)
    {
        double dblRcond;
        int ret = iInvertMatrixM(_iRows1, _iCols1, _pdblRealOut, 0/* is complex*/, &dblRcond);
        if (ret == -1)
        {
            if (getWarningMode())
            {
                sciprint(_("Warning :\n"));
                sciprint(_("matrix is close to singular or badly scaled. rcond = %1.4E\n"), dblRcond);
                Sciwarning(_("computing least squares solution. (see lsq).\n"));
            }
        }
    }

    *_iComplex = 0;
    return 0;
}

int iPowerRealSquareMatrixByComplexScalar(
    double* _pdblReal1, int _iRows1, int _iCols1,
    double _dblReal2, double _dblImg2,
    double* _pdblRealOut,	double* _pdblImgOut)
{
    return 0;
}

int iPowerComplexSquareMatrixByRealScalar(
    double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
    double _dblReal2,
    double* _pdblRealOut,	double* _pdblImgOut)
{
    int iInv = 0;
    int iExpRef = (int)_dblReal2;
    if (iExpRef < 0)
    {
        //call matrix invetion
        iInv = 1;
        iExpRef = -iExpRef;
    }

    if ((int)_dblReal2 == _dblReal2) //integer exponent
    {
        if (iExpRef == 1)
        {
            int iSize = _iRows1 * _iCols1;
            int iOne = 1;
            C2F(dcopy)(&iSize, _pdblReal1, &iOne, _pdblRealOut, &iOne);
            C2F(dcopy)(&iSize, _pdblImg1, &iOne, _pdblImgOut, &iOne);
        }
        else if (iExpRef == 0)
        {
            int iSize       = _iRows1 * _iCols1;
            int iOne        = 1;
            double dblOne   = 1;
            double dblZero  = 0;
            int iRowp1      = _iRows1 + 1;

            if (C2F(dasum)(&iSize, _pdblReal1, &iOne) == 0)
            {
                //Invalid exponent
                return 1;
            }
            C2F(dset)(&iSize, &dblZero, _pdblRealOut, &iOne);
            C2F(dset)(&_iRows1, &dblOne, _pdblRealOut, &iRowp1);
        }
        else
        {
            int i = 0;
            int iOne = 1;
            int iTwo = 2;
            int iPos = 0;
            int iPrevPos = 0;
            int iSize = _iCols1 * _iCols1;
            int iSize2 = iSize * 2;
            int iInc = (_iCols1 + 1) * 2;

            double alpha[2] = {1., 0.};
            double beta[2] = {0., 0.};

            double* pdblResult = (double*)malloc(iSize2 * sizeof(double));
            double* pdblTmp = (double*)malloc(iSize2 * sizeof(double));
            double* pdblOut = (double*)malloc(iSize2 * sizeof(double));

            // initialize the output as identity matrix
            // because we use this array in the first multiplication.
            memset(pdblOut, 0x00, iSize2 * sizeof(double));
            C2F(dset)(&_iCols1, &alpha[0], pdblOut, &iInc);

            // copy input complex in working array as Z storage.
            C2F(dcopy)(&iSize, _pdblReal1, &iOne, pdblTmp, &iTwo);
            C2F(dcopy)(&iSize, _pdblImg1, &iOne, pdblTmp + 1, &iTwo);

            // find all "1" in binary representation of the power integer.
            // then perform multiplication according the "1" position.
            while (iExpRef != 0)
            {
                if (iExpRef & 1u)
                {
                    // start the loop at the last position
                    // because we keep the last result to perform
                    // the multiplication.
                    for (i = iPrevPos; i < iPos; i++)
                    {
                        double* pdblSwitch;
                        C2F(zgemm)("N", "N", &_iCols1, &_iCols1, &_iCols1, alpha, pdblTmp, &_iCols1,
                                   pdblTmp, &_iCols1, beta, pdblResult, &_iCols1);

                        pdblSwitch = pdblTmp;
                        pdblTmp = pdblResult;
                        pdblResult = pdblSwitch;
                    }

                    iPrevPos = iPos;

                    C2F(zgemm)("N", "N", &_iCols1, &_iCols1, &_iCols1, alpha, pdblTmp, &_iCols1,
                               pdblOut, &_iCols1, beta, pdblResult, &_iCols1);

                    C2F(dcopy)(&iSize2, pdblResult, &iOne, pdblOut, &iOne);
                }

                iPos++;
                iExpRef = iExpRef >> 1;
            }

            C2F(dcopy)(&iSize, pdblOut, &iTwo, _pdblRealOut, &iOne);
            C2F(dcopy)(&iSize, pdblOut + 1, &iTwo, _pdblImgOut, &iOne);

            free(pdblResult);
            free(pdblTmp);
            free(pdblOut);
        }
    }
    else
    {
        //floating point exponent
        return -1; // manage by overload
    }

    if (iInv)
    {
        double dblRcond;
        double* pData = (double*)oGetDoubleComplexFromPointer(_pdblRealOut, _pdblImgOut, _iRows1 * _iCols1);
        int ret = iInvertMatrixM(_iRows1, _iCols1, pData, 1/* is complex*/, &dblRcond);
        if (ret == -1)
        {
            if (getWarningMode())
            {
                sciprint(_("Warning :\n"));
                sciprint(_("matrix is close to singular or badly scaled. rcond = %1.4E\n"), dblRcond);
                Sciwarning(_("computing least squares solution. (see lsq).\n"));
            }
        }

        vGetPointerFromDoubleComplex((doublecomplex*)pData, _iRows1 * _iCols1, _pdblRealOut, _pdblImgOut);
        vFreeDoubleComplexFromPointer((doublecomplex*)pData);
    }

    return 0;
}

int iPowerComplexSquareMatrixByComplexScalar(
    double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
    double _dblReal2, double _dblImg2,
    double* _pdblRealOut,	double* _pdblImgOut)
{
    return 0;
}
