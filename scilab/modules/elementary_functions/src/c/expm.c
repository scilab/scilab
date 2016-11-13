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

#include <stdio.h>
#include <string.h>
#include "expm.h"
#include "basic_functions.h"
#include "elementary_functions.h"
#include "matrix_left_division.h"
#include "matrix_multiplication.h"
#include "matrix_right_division.h"


/*
purpose
	compute the exponential of a matrix a by the pade's
	approximants(subroutine pade).a block diagonalization
	is performed prior call pade.
syntax
	subroutine dexpm1(ia,n,a,ea,iea,w,iw,ierr)

	integer ia,n,iw,ierr
	double precision a,ea,w
	dimension a(ia,n),ea(iea,n),w(*),iw(*)

	ia: the leading dimension of array a.
	n: the order of the matrices a,ea,x,xi .
	a: the real double precision array that contains the n*n matrix a
	ea: the array that contains the n*n exponential of a.
	iea : the leading dimension of array ea
	w : work space array of size: n*(2*ia+2*n+5)
	iw : integer work space array of size 2*n
	ierr: =0 if the prosessus is normal.
		=-1 if n>ia.
		=-2 if the block diagonalization is not possible.
		=-4 if the subroutine dpade can not computes exp(a)
*/
int dexpms(int _iLeadDim, int _iSize, double *_pdblVal, double *_pdblReturn)
{
    int iErr			= 0;
    int iScal			= 1;
    int iIndex1			= 0;
    int iIndex2			= 0;
    int iIndex4			= 0;
    int iIndex5			= 0;
    int iIndex6			= 0;
    int *piWS			= NULL;

    int iEigenReal		= 0, iEigenImg		= 0;
    int iBlockStruc		= 0, iRightReduce	= 0;
    int iInvRightReduce = 0, iScale			= 0;
    int iiPadeWS		= 0, idblPadeWS		= 0;

    double *pdblWS		= NULL;
    double dblANorm		= 0;

    /*global static variable, used to initialization*/
    sdblExpmN			= -1;

    for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
    {
        double dblAlpha	= 0;
        for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
        {
            dblAlpha += dabss(_pdblVal[iIndex2 + _iSize * iIndex1]);
        }
        if (dblAlpha > dblANorm)
        {
            dblANorm = dblAlpha;
        }
    }

    if (dblANorm == 0)
    {
        for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
        {
            _pdblReturn[iIndex1] = 0;
            _pdblReturn[iIndex1 + iIndex1 * _iSize] = 1;
        }
        return 0;
    }

    piWS = (int*)malloc(_iSize * _iSize * sizeof(int));
    pdblWS = (double*)malloc(sizeof(double) * (_iSize * (4 * _iSize + 5)));

    iScale			= 0;
    iRightReduce	= _iSize;
    iInvRightReduce	= iRightReduce + _iLeadDim * _iSize;
    iEigenReal		= iInvRightReduce + _iLeadDim * _iSize;
    iEigenImg		= iEigenReal + _iSize;
    idblPadeWS			= iEigenImg + _iSize;

    iBlockStruc		= 0;
    iiPadeWS		= iBlockStruc + _iSize;

    dblANorm = Max(dblANorm, 1);
    iErr = dbdiaga(_iLeadDim, _iSize, _pdblVal, 0, dblANorm,
                   &pdblWS[iEigenReal], &pdblWS[iEigenImg], &piWS[iBlockStruc],
                   &pdblWS[iRightReduce], &pdblWS[iInvRightReduce], &pdblWS[iScale], 1);

    if (iErr)
    {
        free(piWS);
        free(pdblWS);
        return -2;
    }

    for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
    {
        vDset(_iSize, 0, &_pdblReturn[iIndex1], _iSize);
    }

    //compute the pade's approximants of the block.
    //block origin is shifted before calling pade.

    iIndex1	= 1;
    iIndex2	= 0;

    //loop on the block.
    while (iIndex2 <= _iSize)
    {
        double dblTemp = 0;
        double dblAlpha	= 0;

        iIndex2 += iIndex1;
        if (iIndex2 <= _iSize)
        {
            iIndex1	= piWS[iBlockStruc + iIndex2 - 1];
            if (iIndex1 != 1)
            {
                iIndex4 = iIndex2 + iIndex1;
                for (iIndex5 = iIndex2 ; iIndex5 < iIndex4 ; iIndex5++)
                {
                    dblTemp += pdblWS[iEigenReal - 1 + iIndex5];
                }
                dblTemp /= iIndex1;

                for (iIndex5 = iIndex2 ; iIndex5 < iIndex4 ; iIndex5++)
                {
                    pdblWS[iEigenReal - 1 + iIndex5] -= dblTemp;
                    _pdblVal[iIndex5 + iIndex5 * _iSize] -= dblTemp;
                }
                dblAlpha	= 0;
                for (iIndex5 = iIndex2 ; iIndex5 < iIndex4 ; iIndex5++)
                {
                    double dblTemp1 = pow(pdblWS[iEigenReal - 1 + iIndex5], 2) + pow(pdblWS[iEigenImg - 1 + iIndex5], 2);
                    dblTemp1		= dsqrts(dblTemp1);
                    if (dblTemp1 > dblAlpha)
                    {
                        dblAlpha = dblTemp1;
                    }
                }

                //call pade subroutine.
                iErr = dpades(&_pdblVal[iIndex2 + iIndex2 * _iSize], _iSize, iIndex1, &_pdblReturn[iIndex2 + iIndex2 * _iSize],
                              _iSize, &dblAlpha, &pdblWS[idblPadeWS], &piWS[iiPadeWS]);
                /*
                				C2F(pade)(&_pdblVal[iIndex2 + iIndex2 * _iSize], &_iSize, &iIndex1, &_pdblReturn[iIndex2 + iIndex2 * _iSize],
                					&_iSze, &dblAlpha, &pdblWS[idblPadeWS], &piWS[iiPadeWS], &iErr);
                */
                if (iErr < 0)
                {
                    free(piWS);
                    free(pdblWS);
                    return 0;
                }

                //remove the effect of origin shift on the block.
                dblTemp = dexps(dblTemp);
                for (iIndex5 = iIndex2 ; iIndex5 < iIndex4 ; iIndex5++)
                    for (iIndex6 = iIndex2 ; iIndex6 < iIndex4 ; iIndex6++)
                    {
                        _pdblReturn[iIndex5 + iIndex6 * _iSize] *= dblTemp;
                    }
            }
            else
            {
                _pdblReturn[(iIndex2 - 1) + (iIndex2 - 1) * _iSize] =
                    dexps(_pdblVal[(iIndex2 - 1) + (iIndex2 - 1) * _iSize]);
            }
        }
    }

    //remove the effect of block diagonalization.
    ddmmuls(&pdblWS[iRightReduce], _iSize, _pdblReturn, _iSize, &pdblWS[idblPadeWS], _iSize, _iSize, _iSize, _iSize);
    ddmmuls(&pdblWS[idblPadeWS], _iSize, &pdblWS[iInvRightReduce], _iSize, _pdblReturn, _iSize, _iSize, _iSize, _iSize);

    free(piWS);
    free(pdblWS);
    return 0;
}

/*
purpose
		dbdiag reduces a matrix a to block diagonal form by first
		reducing it to quasi-triangular form by hqror2 and then by
		solving the matrix equation -a11*p+p*a22=a12 to introduce zeros
		above the diagonal.
		right transformation is factored : p*d*u*y ;where:
			p is a permutation matrix and d positive diagonal matrix
			u is orthogonal and y block upper triangular with identity
				blocks on the diagonal

	starred parameters are altered by the subroutine


		*a		an array that initially contains the m x n matrix
				to be reduced. on return,  see job

		lda		the leading dimension of array a. and array x,xi.
		n		the order of the matrices a,x,xi

		epsshr	the minimal conditionnement allowed for linear sytems

		rmax	the maximum size allowed for any element of the
				transformations.

		*er		a singly subscripted real array containing the real
				parts of the eigenvalues.

		*ei		a singly subscripted real array containing the imaginary
				parts of the eigenvalues.

		*bs		a singly subscripted integer array that contains block
				structure information.  if there is a block of order
				k starting at a(l,l) in the output matrix a, then
				bs(l) contains the positive integer k, bs(l+1) contains
				-(k-1), bs(la+2) = -(k-2), ..., bs(l+k-1) = -1.
				thus a positive integer in the l-th entry of bs
				indicates a new block of order bs(l) starting at a(l,l).

		*x		contains,  either right reducing transformation u*y,
				either orthogonal tranformation u (see job)

		*xi		xi contains the inverse reducing matrix transformation
				or y matrix (see job)

		*scale	contains the scale factor and definitions of p size(n)

		job		integer parametre specifying outputed transformations
				job=0 : a contains block diagonal form
						x right transformation
						xi dummy variable
				job=1 : like job=0 and xi contain x**-1
				job=2	a contains block diagonal form
						x contains u and xi contain y
				job=3 : a contains:
							-block diagonal form in the diagonal blocks
							-a factorisation of y in the upper triangular
						x contains u
						xi dummy
				*fail   a logical variable which is false on normal return and
						true if there is any error in bdiag.
*/
int dbdiaga(int _iLeadDim, int _iSize, double *_pdblVal, double _dblEps,
            double _dblMax, double *_pdblEigenReal, double *_pdblEigenImg,
            int *_piBlockStruc, double *_pdblRightReduce, double *_pdblInvRightReduce,
            double *_pdblScale, int _iMode)
{
    int iErr			= 0;
    int iIndex1			= 0;
    int iIndex2			= 0;
    int iIndex3			= 0;
    int iLow			= 0;
    int iHigh			= 0;
    int iLoop11			= 0;
    int iLoop22			= 0;
    int iSize11			= 0;
    int iSize22			= 0;
    int iLoop22m1		= 0;
    int iK				= 0;
    int iL				= 0;
    int inK				= 0;
    int iKm1			= 0;
    int iKm2			= 0;

    int iMinusOne		= -1;
    int iZero			= 0;
    int iOne			= 1;
    int iTwo			= 2;

    double dblMinusOne	= -1;
    double dblZero		= 0;
    double dblOne		= 1;
    double dblTwo		= 2;

    double dblEps		= 0;
    double dblAvgReal	= 0;
    double dblAvgImg	= 0;
    double dblD			= 0;

    for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
    {
        double dblTemp	= 0;
        for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
        {
            dblTemp += dabss(_pdblVal[iIndex2 + _iSize * iIndex1]);
        }

        dblEps = Max(dblEps, dblTemp);
    }
    if (dblEps == 0)
    {
        dblEps = 1;
    }

    dblEps = nc_eps_machine() * dblEps;

    //convert a to upper hessenberg form.
    dbalancs(_iLeadDim, _iSize, _pdblVal, &iLow, &iHigh, _pdblScale);
    dorthess(_iLeadDim, _iSize, iLow, iHigh, _pdblVal, _pdblEigenReal);
    dortrans(_iLeadDim, _iSize, iLow, iHigh, _pdblVal, _pdblEigenReal, _pdblRightReduce);

    //convert a to quasi-upper triangular form by qr method.
    iErr = dhqror2s(_iLeadDim, _iSize, 1, _iSize, _pdblVal, _pdblEigenReal, _pdblEigenImg, _pdblRightReduce, 21);

    //check to see if hqror2 failed in computing any eigenvalue
    if (iErr != 0)
    {
        return iErr;
    }

    /*
    	reduce a to block diagonal form


    	segment a into 4 matrices: a11, a da11 x da11 block
    	whose (1,1)-element is at a(l11,l11))  a22, a da22 x da22
    	block whose (1,1)-element is at a(l22,l22)) a12,
    	a da11 x da22 block whose (1,1)-element is at a(l11,l22))
    	and a21, a da22 x da11 block = 0 whose (1,1)-
    	element is at a(l22,l11).



    	this loop uses l11 as loop index and splits off a block
    	starting at a(l11,l11).
    */
    iLoop11	= 1;

    //L40
    while (iLoop11 <= _iSize)
    {
        iLoop22 = iLoop11;

        //this loop uses da11 as loop variable and attempts to split
        //off a block of size da11 starting at a(l11,l11)

        //sorry for "while(1)" but the original source code in fortran
        //do loop until goto outside the loop
        //L50
        while (1)
        {
            if (iLoop22 == iLoop11)
            {
                iSize11		= 1;
                if (iLoop11 != (_iSize - 1))
                    if (dabss(_pdblVal[iLoop11 + (iLoop11 - 1) * _iSize]) > dblEps)
                    {
                        iSize11 = 2;
                    }

                iLoop22		= iLoop11 + iSize11;
                iLoop22m1	= iLoop22 - 1;

            }
            else
            {
                //compute the average of the eigenvalues in a11
                dblAvgReal	= 0;
                dblAvgImg	= 0;
                for (iIndex1 = iLoop11 - 1 ; iIndex1 < iLoop22m1 - 1 ; iIndex1++)
                {
                    dblAvgReal	+= _pdblEigenReal[iIndex1];
                    dblAvgImg	+= dabss(_pdblEigenImg[iIndex1]);
                }
                dblAvgReal	/= iSize11;
                dblAvgImg	/= iSize11;

                //loop on eigenvalues of a22 to find the one closest to the av
                dblD		= pow(dblAvgReal - _pdblEigenReal[iLoop22], 2) + pow(dblAvgImg - _pdblEigenImg[iLoop22], 2);
                iK			= iLoop22;
                iL			= iK + 1;

                if (iLoop22 != (_iSize - 1))
                    if (dabss(_pdblVal[(iLoop22 + 1) + iLoop22 * _iSize]) > dblEps)
                    {
                        iL	= iLoop22 + 2;
                    }

                //L71

                //L80
                while (iL < _iSize)
                {
                    double dblTemp = pow(dblAvgReal - _pdblEigenReal[iL], 2) + pow(dblAvgImg - _pdblEigenImg[iL], 2);
                    if (dblTemp < dblD)
                    {
                        iK		= iL;
                        dblD	= dblTemp;
                    }
                    iL++;
                    if (iL <= _iSize)
                        if (dabss(_pdblVal[iL + (iL * - 1) * _iSize]) > dblEps)
                        {
                            iL++;
                        }
                }
                //loop to move the eigenvalue just located
                //into first position of block a22.
                if (iK != (_iSize - 1) && dabss(_pdblVal[(iK + 1) + iK * _iSize] > dblEps))
                {
                    inK	= 1;
                    while (iK > iLoop22)
                    {
                        iKm1 = iK - 1;
                        if (dabss(_pdblVal[iKm1 + (iK - 2) * _iSize]) >= dblEps)
                        {
                            double dblE1 = 0, dblE2 = 0;
                            //we're swapping the closest block with a 2 x 2
                            iKm2 = iK - 2;
                            //C2F(exch)(&_iLeadDim, &_iSize, _pdblVal, _pdblRightReduce, &iKm2, &iTwo, &iOne);
                            dexchs(_iLeadDim, _iSize, _pdblVal, _pdblRightReduce, iKm2, 2, 1);
                            //try to split this block into 2 real eigenvalues
                            //C2F(split)(_pdblVal, _pdblRightReduce, &_iSize, &iKm1, &dblE1, &dblE2, &_iLeadDim, &_iLeadDim);
                            dsplits(_pdblVal, _pdblRightReduce, _iSize, iKm1, &dblE1, &dblE2, _iLeadDim, _iLeadDim);
                        }
                        iK--;
                    }
                }

                inK	= 2;
            }

            //L240
            if (iLoop22 <= _iSize)
            {
                //attempt to split off a block of size da11.
                iSize22	= _iSize - iLoop22 + 1;
                //save a12 in its transpose form in block a21.

                for (iIndex1 = iLoop11 - 1 ; iIndex1 < iLoop22m1 ; iIndex1++)
                {
                    for (iIndex2 = iLoop22 - 1 ; iIndex2 < _iSize ; iIndex2++)
                    {
                        _pdblVal[iIndex2 + iIndex1 * _iSize] = _pdblVal[iIndex1 + iIndex2 * _iSize];
                    }
                }

                //convert a11 to lower quasi-triangular and multiply it by -1 and
                //a12 appropriately (for solving -a11*p+p*a22=a12).
                C2F(dad)(_pdblVal, &_iLeadDim, &iLoop11, &iLoop22m1, &iLoop11, &_iSize, &dblOne, &iZero);
                C2F(dad)(_pdblVal, &_iLeadDim, &iLoop11, &iLoop22m1, &iLoop11, &iLoop22m1, &dblMinusOne, &iOne);

                //solve -a11*p + p*a22 = a12.

                C2F(shrslv)(&_pdblVal[(iLoop11 - 1) + (iLoop11 - 1) * _iSize], &_pdblVal[(iLoop22 - 1) + (iLoop22 - 1) * _iSize],
                            &_pdblVal[(iLoop11 - 1) + (iLoop22 - 1) * _iSize], &iSize11, &iSize22,
                            &_iLeadDim, &_iLeadDim, &_iLeadDim, &dblEps, &_dblEps, &_dblMax, &iErr);

                if (iErr)
                {
                    //change a11 back to upper quasi-triangular.
                    C2F(dad)(_pdblVal, &_iLeadDim, &iLoop11, &iLoop22m1, &iLoop11, &iLoop22m1, &dblOne, &iOne);
                    C2F(dad)(_pdblVal, &_iLeadDim, &iLoop11, &iLoop22m1, &iLoop11, &iLoop22m1, &dblMinusOne, &iOne);

                    //was unable to solve for p - try again
                    //move saved a12 back into its correct position.
                    for (iIndex1 = iLoop11 ; iIndex1 < iLoop22m1 ; iIndex1++)
                    {
                        for (iIndex2 = iLoop22 ; iIndex2 < _iSize ; iIndex2++)
                        {
                            _pdblVal[iIndex1 + iIndex2 * _iSize]	= _pdblVal[iIndex2 + iIndex1 * _iSize];
                            _pdblVal[iIndex2 + iIndex1 * _iSize]	= 0;
                        }
                    }
                }//if iErr
                else
                {
                    break;
                }
            }//if iLoop22 <= _iSize
            else
            {
                break;
            }
        }// while 1
        //L290
        //change solution to p to proper form.
        if (iLoop22 <= _iSize)
        {
            C2F(dad)(_pdblVal, &_iLeadDim, &iLoop11, &iLoop22m1, &iLoop11, &_iSize, &dblOne, &iZero);
            C2F(dad)(_pdblVal, &_iLeadDim, &iLoop11, &iLoop22m1, &iLoop11, &iLoop22m1, &dblMinusOne, &iOne);
        }

        //store block size in array bs.
        _piBlockStruc[iLoop11 - 1]	= iSize11;
        iIndex1 = iSize11 - 1;
        if (iIndex1 != 0)
        {
            for (iIndex2 = 0 ; iIndex2 < iIndex1; iIndex2++)
            {
                _piBlockStruc[(iLoop11 - 1) + iIndex2] = -(iSize11 - iIndex2);
            }
        }
        iLoop11 = iLoop22;
    }//while iLoop11 < _iSize (L350)

    //set transformations matrices as required
    if (_iMode == 3)
    {
        return 0;
    }

    switch (_iMode)
    {
        case 2 :	//extract non orthogonal tranformation from a
            for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
            {
                vDset(_iSize, 0, &_pdblInvRightReduce[iIndex1 * _iSize], 1);
            }
            vDset(_iSize, 1, _pdblInvRightReduce, _iLeadDim + 1);

            iLoop22 = 1;
            while (iLoop11 <= _iSize)
            {
                iLoop11 = iLoop22;
                iLoop22 = iLoop11 + _piBlockStruc[iLoop11];
                for (iIndex1 = (iLoop22 - 1) ; iIndex1 < _iSize ; iIndex1++)
                {
                    for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
                    {
                        int iTemp1 = iLoop22 - iLoop11;
                        _pdblInvRightReduce[iIndex2 + iIndex1 * _iSize] +=
                            C2F(ddot)(&iTemp1, &_pdblInvRightReduce[iIndex2 + (iLoop11 - 1) * _iSize], &_iLeadDim, &_pdblVal[(iLoop11 - 1) + iIndex1 * _iSize], &iOne);
                    }
                }
            }
            break;
        case 1 :	//compute inverse transformation
            for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
            {
                for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
                {
                    _pdblInvRightReduce[iIndex1 + iIndex2 * _iSize] = _pdblRightReduce[iIndex2 + iIndex1 * _iSize];
                }
            }
            iLoop22 = 1;

            while (iLoop22 <= _iSize)
            {
                iLoop11	= iLoop22;
                iLoop22	= iLoop11 + _piBlockStruc[iLoop11 - 1];
                if (iLoop22 <= _iSize)
                {
                    iLoop22m1	= iLoop22 - 1;

                    for (iIndex1 = (iLoop11 - 1) ; iIndex1 < iLoop22m1 ; iIndex1++)
                    {
                        for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
                        {
                            int iTemp1 = _iSize - iLoop22m1;
                            _pdblInvRightReduce[iIndex1 + iIndex2 * _iSize] -=
                                C2F(ddot)(&iTemp1, &_pdblVal[iIndex1 + (iLoop22 - 1) * _iSize], &_iLeadDim, &_pdblInvRightReduce[(iLoop22 - 1) + iIndex2 * _iSize], &iOne);
                        }
                    }
                }
            }//while(iLoop22 <= _iSize)

            if (iHigh != iLow)
            {
                for (iIndex1 = iLow ; iIndex1 < iHigh ; iIndex1++)
                {
                    double dblTemp = 1.0 / _pdblScale[iIndex1];
                    for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
                    {
                        _pdblInvRightReduce[iIndex2 + iIndex1 * _iSize] *= dblTemp;
                    }
                }
            }

            for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
            {
                iIndex2 = iIndex1;
                if (iIndex2 < iLow || iIndex2 > iHigh)
                {
                    if (iIndex2 < iLow)
                    {
                        iIndex2 = iLow - iIndex1;
                    }

                    iK	= (int)_pdblScale[iIndex2];
                    if (iK != iIndex2)
                    {
                        for (iIndex3 = 0 ; iIndex3 < _iSize ; iIndex3++)
                        {
                            vSwitchVal(_pdblInvRightReduce, iIndex3 + iIndex2 * _iSize, iIndex3 + iK * _iSize);
                        }
                    }
                }
            }
        // Warning, case 1 continue to default process
        //break;
        default :	//compute right transformation
            iLoop22	= 1;
            while (iLoop22 <= _iSize)
            {
                iLoop11	= iLoop22;
                iLoop22	= iLoop11 + _piBlockStruc[iLoop11 - 1];
                if (iLoop11 <= _iSize)
                {
                    for (iIndex1 = (iLoop22 - 1) ; iIndex1 < _iSize ; iIndex1++)
                    {
                        for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
                        {
                            int iTemp1 = iLoop22 - iLoop11;
                            _pdblRightReduce[iIndex2 + iIndex1 * _iSize] +=
                                C2F(ddot)(&iTemp1, &_pdblRightReduce[iIndex2 + (iLoop11 - 1) * _iSize], &_iLeadDim, &_pdblVal[(iLoop11 - 1) + iIndex1 * _iSize], &iOne);
                        }
                    }
                }
            }

            C2F(balbak)(&_iLeadDim, &_iSize, &iLow, &iHigh, _pdblScale, &_iSize, _pdblRightReduce);
            break;
    }

    //set zeros in the matrix a
    iLoop11	= 1;
    iLoop22 = iLoop11 + _piBlockStruc[iLoop11 - 1];
    while (iLoop22 < _iSize)
    {
        iLoop22 = iLoop11 + _piBlockStruc[iLoop11 - 1];
        if (iLoop22 <= _iSize)
        {
            iLoop22m1	= iLoop22 - 1;
            for (iIndex1 = (iLoop11 - 1) ; iIndex1 < iLoop22m1 ; iIndex1++)
            {
                vDset(_iSize - iLoop22m1, 0, &_pdblVal[iIndex1 + (iLoop22 - 1) * _iSize], _iLeadDim);
                vDset(_iSize - iLoop22m1, 0, &_pdblVal[(iLoop22 - 1) + iIndex1 * _iSize], 1);
            }
            iLoop11 = iLoop22;
        }
    }


    return iErr;
}

/*
purpose

     this subroutine balances a real matrix and isolates
     eigenvalues whenever possible.
syntax

     on input:

        nm must be set to the row dimension of two-dimensional
          array parameters as declared in the calling program
          dimension statement;

        n is the order of the matrix;

        a contains the input matrix to be balanced.

     on output:

        a contains the balanced matrix;

        low and igh are two integers such that a(i,j)
          is equal to zero if
           (1) i is greater than j and
           (2) j=1,...,low-1 or i=igh+1,...,n;

        scale contains information determining the
           permutations and scaling factors used.

     suppose that the principal submatrix in rows low through igh
     has been balanced, that p(j) denotes the index interchanged
     with j during the permutation step, and that the elements
     of the diagonal matrix used are denoted by d(i,j).  then
        scale(j) = p(j),    for j = 1,...,low-1
                 = d(j,j),      j = low,...,igh
                 = p(j)         j = igh+1,...,n.
     the order in which the interchanges are made is n to igh+1,
     then 1 to low-1.

     note that 1 is returned for igh if igh is zero formally.

     the algol procedure exc contained in balance appears in
     balanc  in line.  (note that the algol roles of identifiers
     k,l have been reversed.)

originator

     this subroutine is a translation of the algol procedure balance,
     num. math. 13, 293-304(1969) by parlett and reinsch.
     handbook for auto. comp., vol.ii-linear algebra, 315-326(1971).

     questions and comments should be directed to b. s. garbow,
     applied mathematics division, argonne national laboratory

     ------------------------------------------------------------------

     :::::::::: radix is a machine dependent parameter specifying
                the base of the machine floating point representation.
*/
int dbalancs(int _iRows, int _iSize, double *_pdblVal, int *_piLow, int *_piHigh, double *_pdblScale)
{
    double dblRadix		= 2;
    double dblRadix2	= dblRadix * dblRadix;
    int iIndex1			= 0;
    int iIndex2			= 0;
    int iLoop1			= _iSize;
    int iLoop2			= 1;

    //flag
    int bLoop1			= 1;
    int bLoop2			= 1;
    int iNoConv			= 0;

    double dblRowSum	= 0;
    double dblColSum	= 0;
    double dblG			= 0;
    double dblF			= 0;
    double dblS			= 0;

    //go to 100
    while (bLoop1)
    {
        int iTemp = 0;
        bLoop1 = 0;
        for (iIndex1 = 0 ; iIndex1 < iLoop1 ; iIndex1++)
        {
            iTemp = iLoop1 - iIndex1 - 1;
            for (iIndex2 = iLoop2 - 1 ; iIndex2 < iLoop1 ; iIndex2++)
            {
                if (iIndex2 == iTemp)
                {
                    continue;
                }
                if (_pdblVal[iTemp + iIndex2 * _iSize] != 0)
                {
                    break;
                }
            }
            if (iIndex2 < iLoop1 && iIndex1 < iLoop1 && _pdblVal[iTemp + iIndex2 * _iSize] != 0)
            {
                continue;
            }
            bLoop1 = 1;
            break;
        }
        if (bLoop1)
        {
            vExchangeVal(	_pdblScale, _pdblVal,
                            0, iLoop1 - 1,
                            iLoop2 - 1, _iSize,
                            _iSize, iTemp, iLoop1 - 1);
            if (iLoop1 == 1)
            {
                *_piLow		= iLoop2;
                *_piHigh	= iLoop1;
                return 0;
            }
            else
            {
                iLoop1--;
            }
        }
    }

    while (bLoop2)
    {
        int iTemp = 0;
        bLoop2 = 0;
        for (iIndex1 = iLoop2 - 1 ; iIndex1 < iLoop1 ; iIndex1++)
        {
            for (iIndex2 = iLoop2 - 1 ; iIndex2 < iLoop1 ; iIndex2++)
            {
                if (iIndex2 == iIndex1)
                {
                    continue;
                }
                if (_pdblVal[iIndex2 + iIndex1 * _iSize] != 0)
                {
                    break;
                }
            }
            if (iIndex2 < iLoop1 && iIndex1 < iLoop1 && _pdblVal[iIndex2 + iIndex1 * _iSize] != 0)
            {
                continue;
            }
            bLoop2 = 1;
            break;
        }
        if (bLoop2)
        {
            vExchangeVal(	_pdblScale, _pdblVal,
                            0, iLoop1 - 1,
                            iLoop2 - 1, _iSize,
                            _iSize, iIndex1, iLoop2 - 1);

            iLoop2++;
        }
    }

    for (iIndex1 = iLoop2 - 1 ; iIndex1 < iLoop1 ; iIndex1++)
    {
        _pdblScale[iIndex1] = 1;
    }

    do
    {
        iNoConv = 0;
        for (iIndex1 = iLoop2 - 1 ; iIndex1 < iLoop1 ; iIndex1++)
        {
            dblRowSum = 0;
            dblColSum = 0;

            for (iIndex2 = iLoop2 - 1 ; iIndex2 < iLoop1 ; iIndex2++)
            {
                dblColSum += dabss(_pdblVal[iIndex2 + iIndex1 * _iSize]);
                dblRowSum += dabss(_pdblVal[iIndex1 + iIndex2 * _iSize]);
            }

            //:::::::::: guard against zero c or r due to underflow ::::::::::
            if (dblRowSum == 0 || dblColSum == 0)
            {
                continue;
            }

            dblG	= dblRowSum / dblRadix;
            dblF	= 1;
            dblS	= dblColSum + dblRowSum;

            while (dblColSum < dblG)
            {
                dblF		*= dblRadix;
                dblColSum	*= dblRadix2;
            }

            dblG	= dblRowSum * dblRadix;

            while (dblColSum >= dblG)
            {
                dblF		/= dblRadix;
                dblColSum	/= dblRadix2;
            }

            //:::::::::: now balance ::::::::::
            if ( ((dblColSum + dblRowSum) / dblF) >= (0.95 * dblS))
            {
                continue;
            }

            dblG				= 1 / dblF;
            _pdblScale[iIndex1] *= dblF;
            iNoConv				= 1;

            for (iIndex2 = iLoop2 - 1 ; iIndex2 < _iSize ; iIndex2++)
            {
                _pdblVal[iIndex1 + iIndex2 * _iSize]	*= dblG;
            }

            for (iIndex2 = 0 ; iIndex2 < iLoop1 ; iIndex2++)
            {
                _pdblVal[iIndex2 + iIndex1 * _iSize]	*= dblF;
            }
        }
    }
    while (iNoConv == 1);

    *_piLow		= iLoop2; //Check if this value is use like array index or not ( 0 indexed or 1 indexed )
    *_piHigh	= iLoop1; //Check if this value is use like array index or not ( 0 indexed or 1 indexed )

    //:::::::::: last card of balanc ::::::::::
    return 0;
}

void vExchangeVal(double *_pdblScale, double *_pdblVal,
                  int _iStart1, int _iEnd1,
                  int _iStart2, int _iEnd2,
                  int _iSize, int _iCoord1, int _iCoord2)
{
    int iIndex			= 0;

    _pdblScale[_iEnd1]	= _iCoord1;
    if (_iCoord1 == _iEnd1)
    {
        return;
    }
    for (iIndex = _iStart1 ; iIndex <= _iEnd1 ; iIndex++)
        vSwitchVal(		_pdblVal,
                        iIndex + _iCoord1 * _iSize,
                        iIndex + _iCoord2 * _iSize);

    for (iIndex = _iStart2 ; iIndex < _iEnd2 ; iIndex++)
        vSwitchVal(		_pdblVal,
                        _iCoord1 + iIndex * _iSize,
                        _iCoord2 + iIndex * _iSize);
}

void vSwitchVal(double *_pdblVal, int _iPos1, int _iPos2)
{
    double dblTemp		= 0;

    dblTemp				= _pdblVal[_iPos1];
    _pdblVal[_iPos1]	= _pdblVal[_iPos2];
    _pdblVal[_iPos2]	= dblTemp;
}

/*
 purpose

    given a real general matrix, this subroutine
    reduces a submatrix situated in rows and columns
    low through igh to upper hessenberg form by
    orthogonal similarity transformations.

 syntax

     subroutine orthes(nm,n,low,igh,a,ort)

    on input:

       nm must be set to the row dimension of two-dimensional
         array parameters as declared in the calling program
         dimension statement;

       n is the order of the matrix;

       low and igh are integers determined by the balancing
         subroutine  balanc.  if  balanc  has not been used,
         set low=1, igh=n;

       a contains the input matrix.

    on output:

       a contains the hessenberg matrix.  information about
         the orthogonal transformations used in the reduction
         is stored in the remaining triangle under the
         hessenberg matrix;

       ort contains further information about the transformations.
         only elements low through igh are used.

originator

    this subroutine is a translation of the algol procedure orthes,
    num. math. 12, 349-368(1968) by martin and wilkinson.
    handbook for auto. comp., vol.ii-linear algebra, 339-358(1971).
    questions and comments should be directed to b. s. garbow,
    applied mathematics division, argonne national laboratory
*/
int dorthess(int _iLead, int _iSize, int _iLow, int _iHigh, double *_pdblVal, double *_pdblOrt)
{
    int iIndex1		= 0;
    int iIndex2		= 0;
    int iIndex3		= 0;

    int iNewHigh	= _iHigh - 1;
    int iNewLow		= _iLow + 1;

    if (iNewHigh < iNewLow)
    {
        return 0;
    }

    for (iIndex1 = iNewLow - 1 ; iIndex1 < iNewHigh ; iIndex1++)
    {
        double dblH			= 0;
        double dblG			= 0;
        double dblScale		= 0;
        int iOffMax			= 0;

        _pdblOrt[iIndex1]	= 0;

        //:::::::::: scale column (algol tol then not needed) ::::::::::
        for (iIndex2 = iIndex1 ; iIndex2 < _iHigh; iIndex2++)
        {
            dblScale += dabss(_pdblVal[iIndex2 + (iIndex1 - 1) * _iSize]);
        }

        if (dblScale == 0)
        {
            continue;
        }

        iOffMax = iIndex1 + _iHigh;

        //:::::::::: for i=igh step -1 until m do -- ::::::::::
        for (iIndex2 = iIndex1 ; iIndex2 < _iHigh; iIndex2++)
        {
            iIndex3				= iOffMax - (iIndex2 + 1);
            _pdblOrt[iIndex3]	= _pdblVal[iIndex3 + (iIndex1 - 1) * _iSize] / dblScale;
            dblH				+= _pdblOrt[iIndex3] * _pdblOrt[iIndex3];
        }

        dblG				= -dsigns(dsqrts(dblH), _pdblOrt[iIndex1]);
        dblH				-= _pdblOrt[iIndex1] * dblG;
        _pdblOrt[iIndex1]	-= dblG;

        //:::::::::: form (i-(u*ut)/h) * a ::::::::::
        for (iIndex2 = iIndex1 ; iIndex2 < _iSize; iIndex2++)
        {
            double dblF = 0;

            //:::::::::: for i=igh step -1 until m do -- ::::::::::
            for (iIndex3 = iIndex1; iIndex3 < _iHigh; iIndex3++)
            {
                int iTemp	= iOffMax - (iIndex3 + 1);
                dblF		+= _pdblOrt[iTemp] * _pdblVal[iTemp + iIndex2 * _iSize];
            }

            dblF /= dblH;

            for (iIndex3 = iIndex1; iIndex3 < _iHigh; iIndex3++)
            {
                _pdblVal[iIndex3 + iIndex2 * _iSize] -= dblF * _pdblOrt[iIndex3];
            }
        }

        //:::::::::: form (i-(u*ut)/h)*a*(i-(u*ut)/h) ::::::::::
        for (iIndex2 = 0 ; iIndex2 < _iHigh; iIndex2++)
        {
            double dblF = 0;

            //:::::::::: for j=igh step -1 until m do -- ::::::::::
            for (iIndex3 = iIndex1; iIndex3 < _iHigh; iIndex3++)
            {
                int iTemp	= iOffMax - (iIndex3 + 1);
                dblF		+= _pdblOrt[iTemp] * _pdblVal[iIndex2 + iTemp * _iSize];
            }

            dblF /= dblH;

            for (iIndex3 = iIndex1; iIndex3 < _iHigh; iIndex3++)
            {
                _pdblVal[iIndex2 + iIndex3 * _iSize] -= dblF * _pdblOrt[iIndex3];
            }
        }

        _pdblOrt[iIndex1]							*= dblScale;
        _pdblVal[iIndex1 + (iIndex1 - 1) * _iSize]	= dblScale * dblG;
    }
    //:::::::::: last card of orthes ::::::::::
    return 0;
}

/*
purpose

    this subroutine accumulates the orthogonal similarity
    transformations used in the reduction of a real general
    matrix to upper hessenberg form by  orthes.

syntax

     subroutine ortran(nm,n,low,igh,a,ort,z)

    on input:

       nm must be set to the row dimension of two-dimensional
         array parameters as declared in the calling program
         dimension statement;

       n is the order of the matrix;

       low and igh are integers determined by the balancing
         subroutine  balanc.  if  balanc  has not been used,
         set low=1, igh=n;

       a contains information about the orthogonal trans-
         formations used in the reduction by  orthes
         in its strict lower triangle;

       ort contains further information about the trans-
         formations used in the reduction by  orthes.
         only elements low through igh are used.

    on output:

       z contains the transformation matrix produced in the
         reduction by  orthes;

       ort has been altered.

originator

    this subroutine is a translation of the algol procedure ortrans,
    num. math. 16, 181-204(1970) by peters and wilkinson.
    handbook for auto. comp., vol.ii-linear algebra, 372-395(1971).

    questions and comments should be directed to b. s. garbow,
    applied mathematics division, argonne national laboratory
*/
int dortrans(int _iLead, int _iSize, int _iLow, int _iHigh, double *_pdblVal, double *_pdblOrt, double *_pdblTrans)
{
    int iIndex1 = 0;
    int iIndex2	= 0;
    int iIndex3	= 0;
    int iLoop = 0;

    for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
    {
        for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
        {
            _pdblTrans[iIndex1 + iIndex2 * _iSize] = 0;
        }

        _pdblTrans[iIndex1 + iIndex1 * _iSize] = 1;
    }

    iLoop = _iHigh - _iLow - 1;
    if (iLoop < 0)
    {
        return 0;
    }

    //:::::::::: for mp=igh-1 step -1 until low+1 do -- ::::::::::
    for (iIndex1 = 0 ; iIndex1 < iLoop ; iIndex1++)
    {
        int iTemp	= _iHigh - (iIndex1 + 1) - 1;

        if (_pdblVal[iTemp + (iTemp - 1) * _iSize] == 0)
        {
            continue;
        }

        for (iIndex2 = iTemp + 1 ;  iIndex2 < _iHigh ; iIndex2++)
        {
            _pdblOrt[iIndex2] = _pdblVal[iIndex2 + (iTemp - 1) * _iSize];
        }

        for (iIndex2 = iTemp ;  iIndex2 < _iHigh ; iIndex2++)
        {
            double dblG	= 0;

            for (iIndex3 = iTemp ;  iIndex3 < _iHigh ; iIndex3++)
            {
                dblG	+= _pdblOrt[iIndex3] * _pdblTrans[iIndex3 + iIndex2 * _iSize];
            }

            //::::::::::divisor below is negative of h formed in orthes.
            //::::::::::double division avoids possible underflow
            dblG	= (dblG / _pdblOrt[iTemp] ) / _pdblVal[iTemp + (iTemp - 1) * _iSize];

            for (iIndex3 = iTemp ;  iIndex3 < _iHigh ; iIndex3++)
            {
                _pdblTrans[iIndex3 + iIndex2 * _iSize]	+= dblG * _pdblOrt[iIndex3];
            }

        }
    }

    return 0;
}

/*

	this subroutine is a translation of the algol procedure hqr2,
	num. math. 16, 181-204(1970) by peters and wilkinson.
	handbook for auto. comp., vol.ii-linear algebra, 372-395(1971).

cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

    MODIFICATIONS WRT EISPACK VERSION
    ---------------------------------
      1. 1x1 and 2x2 diagonal blocks are clearly isolated by
         forcing subdiagonal entries to zero
      2. Merging of hqr/hqr2 driven by a job parameter

cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

    This subroutine finds the eigenvalues of a real upper
    hessenberg matrix by the qr method. In addition, the
    orthogonal transformation leading to the Schur form is
    accumulated

    on input

       nm must be set to the row dimension of two-dimensional
         array parameters as declared in the calling program
         dimension statement.

       n is the order of the matrix.

       low and igh are integers determined by the balancing
         subroutine  balanc.  if  balanc  has not been used,
         set low=1, igh=n.

       h contains the upper hessenberg matrix.

       z contains the transformation matrix produced by  eltran
         after the reduction by  elmhes, or by  ortran  after the
         reduction by  orthes, if performed.  if the eigenvectors
         of the hessenberg matrix are desired, z must contain the
         identity matrix.

       job  has the decimal decomposition xy;
           if x=0 hqror2 compute eigen-decomposition of h
           if x=1 hqror2 computes schur decomposition of h
           if x=2 eigenvalues are computed via schur decomposition
           if y=0 coordinate transformation is not accumulated
           if y=1 coordinate transformation is accumulated


    on output

       h contains the Schur form

       wr and wi contain the real and imaginary parts,
         respectively, of the eigenvalues.  the eigenvalues
         are unordered except that complex conjugate pairs
         of values appear consecutively with the eigenvalue
         having the positive imaginary part first.  if an
         error exit is made, the eigenvalues should be correct
         for indices ierr+1,...,n.

       z contains the orthogonal transformation to the real schur
         form. If an error exit is made, z may be incorrect.

       ierr is set to
         zero       for normal return,
         j          if the limit of 30*n iterations is exhausted
                    while the j-th eigenvalue is being sought.

    calls cdiv for complex division.

    questions and comments should be directed to burton s. garbow,
    mathematics and computer science div, argonne national laboratory

    this version dated august 1983.
*/
int dhqror2s(int _iLead, int _iSize, int _iLow, int _iHigh,
             double *_pdblHessUp, double *_pdblEigenReal, double *_pdblEigenImg,
             double *_pdblTrans, int _iMode)
{
    //some loop ...
    int iIndex1		= 0;
    int iIndex2		= 0;
    int iIndex3		= 0;
    int iIndex4		= 0;
    int iIndex5		= 0;
    int iIndex6		= 0;
    int iIndex7		= 0;
    int iIndex8		= 0;
    int iIndex9		= 0;
    int iIndex10	= 0;
    int iIndex11	= 0;
    int iIndex12	= 0;
    int iIndex13	= 0;
    int iIndex14	= 0;
    int iIndex15	= 0;
    int iIndex16	= 0;
    int iIndex17	= 0;
    int iIndex18	= 0;
    int iIndex19	= 0;
    int iIndex20	= 0;
    int iIndex21	= 0;
    int iIndex22	= 0;
    int iIndex23	= 0;
    int iIndex24	= 0;
    int iIndex25	= 0;
    int iIndex26	= 0;
    int iIndex27	= 0;
    int iIndex28	= 0;
    int iIndex29	= 0;
    int iIndex30	= 0;
    int iIndex31	= 0;
    int iIndex32	= 0;
    int iIndex33	= 0;

    int iLoop		= 0;
    int iPow10		= 0;
    int iModulo		= 0;
    int iOffset		= 0;
    int iMaxLoop	= 0;

    int iIts		= 0;
    int iCoord1		= iOffset - 1;
    int iCoord2		= iCoord1 - 1;
    int iCoord3		= 0;
    int iLow		= _iLow - 1;
    int iHigh		= _iHigh - 1;

    double dblNorm	= 0;
    double dblEps = nc_eps_machine();

    double dblP		= 0;
    double dblQ		= 0;
    double dblR		= 0;
    double dblS		= 0;
    double dblT		= 0;
    double dblW		= 0;
    double dblX		= 0;
    double dblY		= 0;
    double dblZZ	= 0;

    //I keep old source code, just for fun :
    //jx=job/10
    //jy=job-10*jx
    iPow10			= _iMode / 10;
    iModulo			= _iMode % 10;

    //.......... store roots isolated by balanc
    //				and compute matrix norm ..........
    for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
    {
        for (iIndex2 = iLoop ; iIndex2 < _iSize ; iIndex2++)
        {
            dblNorm += dabss(_pdblHessUp[iIndex1 + iIndex2 * _iSize]);
        }

        iLoop = iIndex1;
        if (iPow10 == 1)
        {
            continue;
        }

        if (iIndex1 >= iLow && iIndex1 <= iHigh)
        {
            continue;
        }

        _pdblEigenReal[iIndex1] = _pdblHessUp[iIndex1 + iIndex1 * _iSize];
        _pdblEigenImg[iIndex1]	= 0;
    }

    iOffset		= iHigh;
    dblT		= 0;
    iMaxLoop	= 30 * _iSize;

    //.......... search for next eigenvalues ..........
L60:
    if (iOffset <= iLow )
    {
        goto L340;
    }

    iIts		= 0;
    iCoord1		= iOffset - 1;
    iCoord2		= iCoord1;

L70:
    //.......... look for single small sub-diagonal element
    //			for l=en step -1 until low do -- ..........
    for (iIndex3 = iLow ; iIndex3 < (iOffset + 1); iIndex3++)
    {
        double dblTemp1 = 0, dblTemp2 = 0;
        iIndex4	= iOffset + iLow - iIndex3;
        if (iIndex4 == iLow)
        {
            break;
        }

        dblS	= dabss(_pdblHessUp[(iIndex4 - 1) + (iIndex4 - 1) * _iSize]) +
                  dabss(_pdblHessUp[iIndex4 + iIndex4 * _iSize]);

        if (dblS == 0)
        {
            dblS = dblNorm;
        }

        dblTemp1	= dblS;
        dblTemp2	= dblTemp1 + dabss(_pdblHessUp[iIndex4 + (iIndex4 - 1) * _iSize]);
        if (dblTemp1 == dblTemp2)
        {
            break;
        }
    }

    //.......... form shift ..........
    dblX	= _pdblHessUp[iOffset + iOffset * _iSize];
    if (iIndex4 == iOffset)
    {
        goto L270;
    }

    dblY	= _pdblHessUp[iCoord1 + iCoord1 * _iSize];
    dblW	= _pdblHessUp[iOffset + iCoord1 * _iSize] * _pdblHessUp[iCoord1 + iOffset * _iSize];
    if (iIndex4 == iCoord1)
    {
        goto L280;
    }

    if (iMaxLoop == 0)
    {
        goto L1000;
    }

    if (iIts == 10 || iIts == 20)
    {
        //.......... form exceptional shift ..........
        dblT += dblX;

        for (iIndex5 = iLow ; iIndex5 < iOffset ; iIndex5++)
        {
            _pdblHessUp[iIndex5 + iIndex5 * _iSize]	-= dblX;
        }

        dblS	= dabss(_pdblHessUp[iOffset + iCoord1 * _iSize]) + dabss(_pdblHessUp[iCoord1 + iCoord2 * _iSize]);
        dblX	= 0.75 * dblS;
        dblY	= dblX;
        dblW	= -0.4375 * dblS * dblS;
    }


    iIts++;
    iMaxLoop--;
    //..........look for two consecutive small
    //			sub-diagonal elements.
    //			for m=en-2 step -1 until l do -- ..........
    for (iIndex6 = iIndex4; iIndex6 < iCoord2; iIndex6++)
    {
        double dblTemp1 = 0, dblTemp2 = 0;

        iIndex7	= iCoord2 + (iIndex4 - 1) - iIndex6; // +/- 1

        dblZZ	= _pdblHessUp[iIndex7 + iIndex7 * _iSize];
        dblR	= dblX - dblZZ;
        dblS	= dblY - dblZZ;
        dblP	= (dblR * dblS - dblW) / _pdblHessUp[(iIndex7 + 1) + iIndex7 * _iSize] + _pdblHessUp[iIndex7 + (iIndex7 + 1) * _iSize];
        dblQ	= _pdblHessUp[(iIndex7 + 1) + (iIndex7 + 1) * _iSize] - dblZZ - dblR - dblS;
        dblR	= _pdblHessUp[(iIndex7 + 2) + (iIndex7 + 1) * _iSize];
        dblS	= dabss(dblP) + dabss(dblQ) + dabss(dblR);
        dblP	/= dblS;
        dblQ	/= dblS;
        dblR	/= dblS;

        if (iIndex7 == iIndex4)
        {
            break;
        }

        dblTemp1 =	dabss(dblP) *
                    (dabss(_pdblHessUp[(iIndex7 - 1) + (iIndex7 - 1) * _iSize]) + dabss(dblZZ)) +
                    dabss(_pdblHessUp[(iIndex7 + 1) + (iIndex7 + 1) * _iSize]);

        dblTemp2 =	dblTemp1 +
                    dabss(_pdblHessUp[iIndex7 + (iIndex7 - 1) * _iSize]) * (dabss(dblQ) + dabss(dblR));
        if (dblTemp1 == dblTemp2)
        {
            break;
        }
    }
    iCoord3	= iIndex7 + 2;

    for (iIndex8 = iCoord3 ; iIndex8 <= iOffset ; iIndex8++)
    {
        _pdblHessUp[iIndex8 + (iIndex8 - 2) * _iSize]	= 0;
        if (iIndex8 != iCoord3)
        {
            _pdblHessUp[iIndex8 + (iIndex8 - 3) * _iSize]	= 0;
        }
    }

    //..........	double qr step involving rows l to en and
    //				columns m to en ..........
    for (iIndex9 = iIndex7 ; iIndex9 <= iCoord1 ; iIndex9++)
    {
        int iNotLast = iIndex9 != iCoord1;
        if (iIndex9 != iIndex7)
        {
            dblP		= _pdblHessUp[iIndex9 + (iIndex9 - 1) * _iSize];
            dblQ		= _pdblHessUp[(iIndex9 + 1) + (iIndex9 - 1) * _iSize];
            dblR		= 0;
            if (iNotLast)
            {
                dblR	= _pdblHessUp[(iIndex9 + 2) + (iIndex9 - 1) * _iSize];
            }
            dblX		= dabss(dblP) + dabss(dblQ) + dabss(dblR);
            if (dblX == 0)
            {
                continue;
            }

            dblP	/= dblX;
            dblQ	/= dblX;
            dblR	/= dblX;
        }

        dblS = dsigns(dsqrts(dblP * dblP + dblQ * dblQ + dblR * dblR), dblP);

        if (iIndex9 != iIndex7)
        {
            _pdblHessUp[iIndex9 + (iIndex9 - 1) * _iSize] = -dblS * dblX;
        }
        else if (iIndex4 == iIndex7)
        {
            _pdblHessUp[iIndex9 + (iIndex9 - 1) * _iSize] *= -1;
        }

        dblP	+= dblS;
        dblX	= dblP / dblS;
        dblY	= dblQ / dblS;
        dblZZ	= dblR / dblS;
        dblQ	/= dblP;
        dblR	/= dblP;

        if (iNotLast == 0)
        {
            //.......... row modification ..........
            for (iIndex10 = iIndex9 ; iIndex10 < _iSize ; iIndex10++)
            {
                dblP	=	_pdblHessUp[iIndex9 + iIndex10 * _iSize] +
                            dblQ * _pdblHessUp[(iIndex9 + 1) + iIndex10 * _iSize];

                _pdblHessUp[iIndex9 + iIndex10 * _iSize]		-= dblP * dblX;
                _pdblHessUp[(iIndex9 + 1) + iIndex10 * _iSize]	-= dblP * dblY;
            }

            iIndex10 = Min(iOffset, iIndex9 + 3);

            //.......... column modification ..........
            for (iIndex11 = 0 ; iIndex11 <= iIndex10 ; iIndex11++)
            {
                dblP	=	dblX * _pdblHessUp[iIndex11 + iIndex9 * _iSize] +
                            dblY * _pdblHessUp[iIndex11 + (iIndex9 + 1) * _iSize];

                _pdblHessUp[iIndex11 + iIndex9 * _iSize]		-= dblP;
                _pdblHessUp[iIndex11 + (iIndex9 + 1) * _iSize]	-= dblP * dblQ;
            }

            if (iModulo == 1)
            {
                //.......... accumulate transformations ..........
                for (iIndex12 = iLow ; iIndex12 <= iHigh ; iIndex12++)
                {
                    dblP	=	dblX * _pdblTrans[iIndex12 + iIndex9 * _iSize] +
                                dblY * _pdblTrans[iIndex12 + (iIndex9 + 1) * _iSize];

                    _pdblTrans[iIndex12 + iIndex9 * _iSize]			-= dblP;
                    _pdblTrans[iIndex12 + (iIndex9 + 1) * _iSize]	-= dblP * dblQ;
                }
            }
        }
        else
        {
            //.......... row modification ..........
            for (iIndex13 = iIndex9 ; iIndex13 < _iSize ; iIndex13++)
            {
                dblP	=	_pdblHessUp[iIndex9 + iIndex13 * _iSize] +
                            dblQ * _pdblHessUp[(iIndex9 + 1) + iIndex13 * _iSize] +
                            dblR * _pdblHessUp[(iIndex9 + 2) + iIndex13 * _iSize];

                _pdblHessUp[iIndex9 + iIndex13 * _iSize]		-= dblP * dblX;
                _pdblHessUp[(iIndex9 + 1) + iIndex13 * _iSize]	-= dblP * dblY;
                _pdblHessUp[(iIndex9 + 2) + iIndex13 * _iSize]	-= dblP * dblZZ;

            }

            iIndex14 = Min(iOffset, iIndex9 + 3);

            //.......... column modification ..........
            for (iIndex15 = 0 ; iIndex15 <= iIndex14 ; iIndex15++)
            {
                dblP	=	dblX * _pdblHessUp[iIndex15 + iIndex9 * _iSize] +
                            dblY * _pdblHessUp[iIndex15 + (iIndex9 + 1) * _iSize] +
                            dblZZ * _pdblHessUp[iIndex15 + (iIndex9 + 2) * _iSize];

                _pdblHessUp[iIndex15 + iIndex9 * _iSize]		-= dblP;
                _pdblHessUp[iIndex15 + (iIndex9 + 1) * _iSize]	-= dblP * dblQ;
                _pdblHessUp[iIndex15 + (iIndex9 + 2) * _iSize]	-= dblP * dblR;

            }

            if (iModulo == 1)
            {
                //.......... accumulate transformations ..........
                for (iIndex16 = iLow ; iIndex16 <= iHigh ; iIndex16++)
                {
                    dblP	=	dblX * _pdblTrans[iIndex16 + iIndex9 * _iSize] +
                                dblY * _pdblTrans[iIndex16 + (iIndex9 + 1) * _iSize] +
                                dblZZ * _pdblTrans[iIndex16 + (iIndex9 + 2) * _iSize];

                    _pdblTrans[iIndex16 + iIndex9 * _iSize]			-= dblP;
                    _pdblTrans[iIndex16 + (iIndex9 + 1) * _iSize]	-= dblP * dblQ;
                    _pdblTrans[iIndex16 + (iIndex9 + 2) * _iSize]	-= dblP * dblR;
                }
            }
        }
    }

    goto L70;

    //.......... one root found ..........
L270:
    _pdblHessUp[iOffset + iOffset * _iSize] = dblX + dblT;
    //ADDED TO MARK BLOCK SEPARATION BY HARD ZEROS
    if (iOffset + 1 < _iSize)
    {
        _pdblHessUp[(iOffset + 1) + (iOffset + 1) * _iSize] = 0;
    }

    if (iPow10 != 1)
    {
        _pdblEigenReal[iOffset] = _pdblHessUp[iOffset + iOffset * _iSize];
        _pdblEigenImg[iOffset]	= 0;
    }
    iOffset = iCoord1;
    goto L60;

    //.......... two roots found ..........
L280:
    dblP									= (dblY - dblX) / 2.0;
    dblQ									= dblP * dblP + dblW;
    dblZZ									= dsqrts(dabss(dblQ));
    _pdblHessUp[iOffset + iOffset * _iSize] = dblX + dblT;
    dblX									= _pdblHessUp[iOffset + iOffset * _iSize];
    _pdblHessUp[iCoord1 + iCoord1 * _iSize]	= dblY + dblT;


    if (dblQ < 0)
    {
        goto L320;
    }

    //.......... real pair ..........
    dblZZ	= dblP + dsigns(dblZZ, dblP);
    if (iPow10 != 1)
    {
        _pdblEigenReal[iCoord1]		= dblX + dblZZ;
        _pdblEigenReal[iOffset]		= _pdblEigenReal[iCoord1];

        if (dblZZ != 0)
        {
            _pdblEigenReal[iOffset]	= dblX - dblW / dblZZ;
        }

        _pdblEigenImg[iCoord1]		= 0;
        _pdblEigenImg[iOffset]		= 0;
    }

    dblX	= _pdblHessUp[iOffset + iCoord1 * _iSize];
    dblS	= dabss(dblX) + dabss(dblZZ);
    dblP	= dblX / dblS;
    dblQ	= dblZZ / dblS;
    dblR	= dsqrts(dblP * dblP + dblQ * dblQ);
    dblP	= dblP / dblR;
    dblQ	= dblQ / dblR;

    //.......... row modification ..........
    for (iIndex17 = iCoord1 ; iIndex17 < _iSize ; iIndex17++)
    {
        dblZZ	= _pdblHessUp[iCoord1 + iIndex17 * _iSize];

        _pdblHessUp[iCoord1 + iIndex17 * _iSize]	= dblQ * dblZZ + dblP * _pdblHessUp[iOffset + iIndex17 * _iSize];
        _pdblHessUp[iOffset + iIndex17 * _iSize]	= dblQ * _pdblHessUp[iOffset + iIndex17 * _iSize] - dblP * dblZZ;
    }

    //.......... column modification ..........
    for (iIndex18 = 0 ; iIndex18 <= iOffset ; iIndex18++)
    {
        dblZZ	= _pdblHessUp[iIndex18 + iCoord1 * _iSize];

        _pdblHessUp[iIndex18 + iCoord1 * _iSize]	= dblQ * dblZZ + dblP * _pdblHessUp[iIndex18 + iOffset * _iSize];
        _pdblHessUp[iIndex18 + iOffset * _iSize]	= dblQ * _pdblHessUp[iIndex18 + iOffset * _iSize] - dblP * dblZZ;
    }

    //.......... accumulate transformations ..........
    if (iModulo == 1)
    {
        for (iIndex19 = iLow ; iIndex19 <= iHigh ; iIndex19++)
        {
            dblZZ	= _pdblTrans[iIndex19 + iCoord1 * _iSize];

            _pdblTrans[iIndex19 + iCoord1 * _iSize]	= dblQ * dblZZ + dblP * _pdblTrans[iIndex19 + iOffset * _iSize];
            _pdblTrans[iIndex19 + iOffset * _iSize]	= dblQ * _pdblTrans[iIndex19 + iOffset * _iSize] - dblP * dblZZ;
        }
    }

    //ADDED TO MARK BLOCK SEPARATION BY HARD ZEROS
    _pdblHessUp[iOffset + iCoord1 * _iSize]			= 0;
    if (iOffset + 1 != _iSize)
    {
        _pdblHessUp[iOffset + 1 + iOffset * _iSize] = 0;
    }

    goto L330;

    //.......... complex pair ..........
L320:
    if (iPow10 != 1)
    {
        _pdblEigenReal[iCoord1]	= dblX + dblP;
        _pdblEigenReal[iOffset]	= dblX + dblP;
        _pdblEigenImg[iCoord1]	= dblZZ;
        _pdblEigenImg[iOffset]	= -dblZZ;
    }

    //ADDED TO MARK BLOCK SEPARATION BY HARD ZEROS
    if (iOffset + 1 <= _iSize)
    {
        _pdblHessUp[iOffset + 1 + iOffset * _iSize] = 0;
    }

L330:
    iOffset	= iCoord2;
    goto L60;

L340:
    if (iPow10 != 0 || dblNorm == 0)
    {
        goto L1001;
    }

    //:::::::::: for en=n step -1 until 1 do -- ::::::::::
    for (iIndex21 = 0 ; iIndex21 < _iSize ; iIndex21++)
    {
        double dblCRES	= 0;

        iOffset	= _iSize + 1 - iIndex21;
        dblP	= _pdblEigenReal[iOffset];
        dblQ	= _pdblEigenImg[iOffset];
        iCoord1	= iOffset - 1;
        dblQ	= dblQ + 1;
        dblCRES	= dblQ - 1;
        if (dblCRES < 0)
        {
            goto L710;
        }
        else if (dblCRES > 0)
        {
            continue;
        }

        //:::::::::: real vector ::::::::::
        iIndex22	= iOffset;
        _pdblHessUp[iOffset + iOffset * _iSize] = 1;
        if (iCoord1 == 0)
        {
            continue;
        }

        //:::::::::: for i=en-1 step -1 until 1 do -- ::::::::::
        for (iIndex23 = 0 ; iIndex23 < iCoord1 ; iIndex23++)
        {
            iIndex24	= iOffset - iIndex23;
            dblW		= _pdblHessUp[iIndex24 + iIndex24 * _iSize] - dblP;
            dblR		= _pdblHessUp[iIndex24 + iOffset * _iSize];
            if (iIndex22 <= iCoord1)
                for (iIndex25 = iIndex22 ; iIndex25 < iCoord1 ; iIndex25++)
                    dblR += _pdblHessUp[iIndex24 + iIndex25 * _iSize] *
                            _pdblHessUp[iIndex25 + iOffset * _iSize];

            if (_pdblEigenImg[iIndex24] >= 0)
            {
                goto L630;
            }
            dblZZ	= dblW;
            dblS	= dblR;
            continue;

L630:
            iIndex22	= iIndex24;
            if (_pdblEigenImg[iIndex24] != 0)
            {
                goto L640;
            }

            dblT		= dblW;
            if (dblW == 0)
            {
                dblT	= dblEps * dblNorm;
            }

            _pdblHessUp[iIndex24 + iOffset * _iSize] = -dblR / dblT;
            continue;

L640:
            //:::::::::: solve real equations ::::::::::
            dblX	= _pdblHessUp[iIndex24 + (iIndex24 + 1) * _iSize];
            dblY	= _pdblHessUp[(iIndex24 + 1) + iIndex24 * _iSize];
            dblQ	=	(_pdblEigenReal[iIndex24] - dblP) *
                        (_pdblEigenReal[iIndex24] - dblP) +
                        _pdblEigenImg[iIndex24] * _pdblEigenImg[iIndex24];
            dblT	= (dblX * dblS - dblZZ * dblR) / dblQ;
            _pdblHessUp[iIndex24 + iOffset * _iSize]	= dblT;

            if (dabss(dblX) > dabss(dblZZ))
            {
                _pdblHessUp[iIndex24 + 1 + iOffset * _iSize] = (-dblR - dblT * dblW) / dblX;
            }
            else
            {
                _pdblHessUp[iIndex24 + 1 + iOffset * _iSize] = (-dblS - dblT * dblY) / dblZZ;
            }
        }
        //:::::::::: end real vector ::::::::::
        continue;
        //:::::::::: complex vector ::::::::::
L710:
        iIndex22	= iCoord1;
        //::::::::::	last vector component chosen imaginary so that
        //				eigenvector matrix is triangular ::::::::::
        if (dabss(_pdblHessUp[iOffset + iCoord1 * _iSize]) > dabss(_pdblHessUp[iCoord1 + iOffset * _iSize]))
        {
            _pdblHessUp[iCoord1 + iCoord1 * _iSize]	=	dblQ / _pdblHessUp[iOffset + iCoord1 * _iSize];
            _pdblHessUp[iCoord1 + iOffset * _iSize]	=	-(_pdblHessUp[iOffset + iOffset * _iSize] - dblP) /
                    _pdblHessUp[iOffset + iCoord1 * _iSize];
        }
        else
        {
            double dblReal	= _pdblHessUp[iCoord1 + iCoord1 * _iSize] - dblP; //z3r
            double dblImg	= 0; //z3i
            double dblCoef	= dblReal * dblReal + dblQ * dblQ; //z3

            _pdblHessUp[iCoord1 + iCoord1 * _iSize]	=
                -_pdblHessUp[iCoord1 + iOffset * _iSize] * dblQ / dblCoef;
            _pdblHessUp[iCoord1 + iOffset * _iSize]	=
                -_pdblHessUp[iCoord1 + iOffset * _iSize] * dblReal / dblCoef;
            _pdblHessUp[iOffset + iCoord1 * _iSize]	= 0;
            _pdblHessUp[iOffset + iOffset * _iSize]	= 1;
            iCoord2	= iCoord1 - 1;

            if (iCoord2 == 0)
            {
                continue;
            }

            //:::::::::: for i=en-2 step -1 until 1 do -- ::::::::::
            for (iIndex25 = 0 ; iIndex25 < iCoord2 ; iIndex25++)
            {
                double dblTemp11	= 0; //r
                double dblTemp12	= 0; //ra
                double dblTemp21	= 0; //s
                double dblTemp22	= 0; //sa

                iIndex26		= iCoord1 - iIndex25;
                dblW			= _pdblHessUp[iIndex24 + iIndex24 * _iSize] - dblP;
                dblTemp12		= 0;
                dblTemp22		= _pdblHessUp[iIndex24 + iOffset * _iSize];

                for (iIndex27 = iIndex22 ; iIndex27 < iCoord1 ; iIndex27++)
                {
                    dblTemp12	+=	_pdblHessUp[iIndex24 + iIndex27 * _iSize] *
                                    _pdblHessUp[iIndex27 + iCoord1 * _iSize];
                    dblTemp22	+=	_pdblHessUp[iIndex24 + iIndex27 * _iSize] *
                                    _pdblHessUp[iIndex27 + iOffset * _iSize];
                }

                if (_pdblEigenImg[iIndex24] < 0)
                {
                    dblZZ		= dblW;
                    dblTemp11	= dblTemp12;
                    dblTemp21	= dblTemp22;
                }
                else
                {

                    if (_pdblEigenImg[iIndex24] == 0)
                    {
                        dblCoef	=	dblW * dblW + dblQ * dblQ;
                        dblReal	= -dblTemp12 * dblW - dblTemp22 * dblQ;
                        dblImg	= dblTemp12 * dblQ - dblTemp22 * dblW;
                        _pdblHessUp[iIndex24 + iCoord1 * _iSize] = dblReal / dblCoef;
                        _pdblHessUp[iIndex24 + iOffset * _iSize] = dblImg / dblCoef;
                    }
                    else
                    {
                        double dblVectReal	= 0;
                        double dblVectImg	= 0;

                        dblX				=	_pdblHessUp[iIndex24 + (iIndex24 + 1) * _iSize];
                        dblY				=	_pdblHessUp[(iIndex24 + 1) + iIndex24 * _iSize];
                        dblVectReal			=	(_pdblEigenReal[iIndex24] - dblP) * (_pdblEigenReal[iIndex24] - dblP) +
                                                _pdblEigenImg[iIndex24] * _pdblEigenImg[iIndex24] - dblQ * dblQ;
                        dblVectImg			=	(_pdblEigenReal[iIndex24] - dblP) * 2 * dblQ;

                        if (dblVectReal == 0 && dblVectImg == 0)
                            dblVectReal		=	dblEps * dblNorm *
                                                (dabss(dblW) + dabss(dblQ) + dabss(dblX) + dabss(dblY) + dabss(dblZZ));

                        dblReal				= dblX * dblR - dblZZ * dblTemp12 + dblQ * dblTemp22;
                        dblImg				= dblX * dblS - dblZZ * dblTemp22 - dblQ * dblTemp12;
                        dblCoef				= dblVectReal * dblVectReal + dblVectImg * dblVectImg;

                        _pdblHessUp[iIndex24 + iCoord1 * _iSize]	= (dblReal * dblVectReal + dblImg * dblVectImg) / dblCoef;
                        _pdblHessUp[iIndex24 + iOffset * _iSize]	= (-dblReal * dblVectImg + dblImg * dblVectReal) / dblCoef;

                        if (dabss(dblX) > dabss(dblZZ) + dabss(dblQ))
                        {
                            _pdblHessUp[iIndex24 + 1 + iCoord1 * _iSize] =
                                (-dblTemp12 - dblW * _pdblHessUp[iIndex24 + iCoord1 * _iSize] + dblQ * _pdblHessUp[iIndex24 + iOffset * _iSize]) / dblX;
                            _pdblHessUp[iIndex24 + 1 + iOffset * _iSize] =
                                (-dblTemp22 - dblW * _pdblHessUp[iIndex24 + iOffset * _iSize] + dblQ * _pdblHessUp[iIndex24 + iCoord1 * _iSize]) / dblX;
                        }
                        else
                        {
                            dblReal	= -dblR - dblY * _pdblHessUp[iIndex24 + iCoord1 * _iSize];
                            dblImg	= -dblS - dblY * _pdblHessUp[iIndex24 + iOffset * _iSize];
                            dblCoef	= dblZZ * dblZZ + dblQ * dblQ;
                            _pdblHessUp[iIndex24 + 1 + iCoord1 * _iSize] = (dblReal * dblZZ + dblImg * dblQ) / dblCoef;
                            _pdblHessUp[iIndex24 + 1 + iOffset * _iSize] = (-dblReal * dblQ + dblImg * dblZZ) / dblCoef;
                        }
                    }
                }
            }
            //:::::::::: end complex vector ::::::::::
        }
        //:::::::::: end back substitution.
    }

    if (iModulo == 0)
    {
        return 0;
    }

    //vectors of isolated roots ::::::::::
    for (iIndex27 = 0 ; iIndex27 < _iSize ; iIndex27++)
    {
        if (iIndex27 >= iLow && iIndex27 <= iHigh)
        {
            continue;
        }

        for (iIndex28 = iIndex27 ; iIndex28 < _iSize ; iIndex28++)
        {
            _pdblTrans[iIndex27 + iIndex28 * _iSize] = _pdblHessUp[iIndex27 + iIndex28 * _iSize];
        }
    }

    //:::::::::: multiply by transformation matrix to give
    //			vectors of original full matrix.
    //			for j=n step -1 until low do -- ::::::::::
    for (iIndex29 = iLow ; iIndex29 < _iSize ; iIndex29++)
    {
        for (iIndex32 = iLow ; iIndex32 < iHigh ; iIndex32++)
        {
            dblZZ = 0;
            for (iIndex33 = iLow ; iIndex33 < iIndex31 ; iIndex33++)
            {
                dblZZ += _pdblTrans[iIndex32 + iIndex33 * _iSize] * _pdblHessUp[iIndex33 + iIndex30 * _iSize];
                _pdblTrans[iIndex32 + iIndex30 * _iSize] = dblZZ;
            }
        }
    }
    //..........	set error -- all eigenvalues have not
    //				converged after 30*n iterations ..........
L1001:
    return iOffset;
L1000:
    return 0;
}
/*--------------------------------------------------------------------------*/

int dpades(double *_pdblVal, int _iLeadDimIn, int _iSize, double *_pdblExp, int _iLeadDimOut, double *_pdblAlpha, double *_pdblWS, int *_piWS)
{
    int iErr		= 0;

    double dblZero	= 0;
    double dblOne	= 1;
    double dblTwo	= 2;
    double dblEfact	= 0;
    double dblNorm	= 0;
    double dblRcon	= 0;

    int iZero	= 0;
    int iMax	= 10;
    int iIndex1 = 0;
    int iIndex2	= 0;
    int iLoop1	= 0;
    int iLoop2	= 0;
    int iSize2	= _iSize * _iSize;

    if (sdblExpmN < 0)
    {
        //compute de pade's aprroximants type which is necessary to obtain
        //machine precision
        C2F(coef)(&iErr);
        if (iErr != 0)
        {
            return iErr;
        }
    }

    iIndex1		= 0;
    dblEfact	= dblOne;

    if (*_pdblAlpha < 1)
    {
        goto L90;
    }

    for (iLoop1 = 0 ; iLoop1 < iMax ; iLoop1++)
    {
        iIndex1++;
        dblEfact *= dblTwo;

        if (*_pdblAlpha <= dblEfact)
        {
            goto L60;
        }
    }

L30:
    iIndex1++;
    dblEfact *= dblTwo;
    for (iLoop1 = 0 ; iLoop1 < _iSize ; iLoop1++)
        for (iLoop2 = 0 ; iLoop2 < _iSize ; iLoop2++)
        {
            _pdblVal[iLoop1 + iLoop2 * _iSize] /= dblTwo;
        }

    dblNorm /= dblTwo;
    goto L115;

    //we find a matrix a'=a*2-m whith a spectral radius smaller than one.
L60:
    for (iLoop1 = 0 ; iLoop1 < _iSize ; iLoop1++)
        for (iLoop2 = 0 ; iLoop2 < _iSize ; iLoop2++)
        {
            _pdblVal[iLoop1 + iLoop2 * _iSize] /= dblEfact;
        }

L90:
    C2F(cerr)(_pdblVal, _pdblWS, &_iLeadDimIn, &_iSize, &sdblExpmN, &iIndex1, &iMax);

    dblNorm	= dblZero;
    for (iLoop1 = 0 ; iLoop1 < _iSize ; iLoop1++)
    {
        *_pdblAlpha = dblZero;
        for (iLoop2 = 0 ; iLoop2 < _iSize ; iLoop2++)
        {
            *_pdblAlpha += dabss(_pdblVal[iLoop1 + iLoop2 * _iSize]);
        }
        if (*_pdblAlpha > dblNorm)
        {
            dblNorm = *_pdblAlpha;
        }
    }

    //compute the inverse of the denominator of dpade's approximants.
L115:
    for (iLoop1 = 0 ; iLoop1 < _iSize ; iLoop1++)
        for (iLoop2 = 0 ; iLoop2 < _iSize ; iLoop2++)
        {
            _pdblExp[iLoop1 + iLoop2 * _iSize] = -_pdblVal[iLoop1 + iLoop2 * _iSize];
        }

    C2F(dclmat)(&_iLeadDimOut, &_iSize, _pdblExp, _pdblWS, &_iSize, &_pdblWS[iSize2 + 1], spdblExpmC, &sdblExpmN);

    //compute de l-u decomposition of n (-a) and the condition numbwk(n2+1)
    //									pp
    C2F(dgeco)(_pdblWS, &_iSize, &_iSize, _piWS, &dblRcon, &_pdblWS[iSize2 + 1]);

    dblRcon = dblRcon * dblRcon * dblRcon * dblRcon;

    if (dblRcon + dblOne <= dblOne && dblNorm > dblOne && iIndex1 < iMax)
    {
        goto L30;
    }

    //compute the numerator of dpade's approximants.
    C2F(dclmat)(&_iLeadDimIn, &_iSize, _pdblVal, _pdblExp, &_pdblWS[iSize2 + 1], spdblExpmC, &sdblExpmN);

    //compute the dpade's approximants by

    //	n (-a) x=n (a)
    //	pp      pp
    for (iLoop1 = 0 ; iLoop1 < _iSize ; iLoop1++)
    {
        C2F(dgesl)(_pdblWS, &_iSize, &_iSize, _piWS, &_pdblExp[iLoop1 * _iSize], &iZero);
    }

    if (iIndex1 != 0)
    {
        //remove the effects of normalization.
        for (iLoop1 = 0 ; iLoop1 < _iSize ; iLoop1++)
        {
            C2F(dmmul)(_pdblExp, &_iLeadDimOut, _pdblExp, &_iLeadDimOut, _pdblWS, &_iSize, &_iSize, &_iSize, &_iSize);
            C2F(dmcopy)(_pdblWS, &_iSize, _pdblExp, &_iLeadDimOut, &_iSize, &_iSize);
        }
    }

    return 0;
}

/*
     PURPOSE
        computes the matrix product C = A * B
            C   =   A   *   B
          (l,n)   (l,m) * (m,n)

     PARAMETERS
        input
        -----
        A : (double) array (l, m) with leading dim na

        B : (double) array (m, n) with leading dim nb

        na, nb, nc, l, m, n : integers

        output
        ------
        C : (double) array (l, n) with leading dim nc

     NOTE
        (original version substituted by a call to the blas dgemm)
*/
void ddmmuls(double *_pdblA, int _iLeadDimA,
             double *_pdblB, int _iLeadDimB,
             double *_pdblOut, int _iLeadDimOut,
             int _iRowsA, int _iColsA, int _iColsB)
{
    char cN			= 'n';
    double dblOne	= 1;
    double dblZero	= 0;

    C2F(dgemm)(&cN, &cN, &_iRowsA, &_iColsB, &_iColsA, &dblOne, _pdblA, &_iLeadDimA, _pdblB, &_iLeadDimB, &dblZero, _pdblOut, &_iLeadDimOut);
}


/*
	purpose
	 given  upper hessenberg matrix a
	 with consecutive ls1xls1 and ls2xls2 diagonal blocks (ls1,ls2.le.2)
	 starting at row/column l, exch produces equivalence transforma-
	 tion zt that exchange the blocks along with their
	 eigenvalues.

	syntax

		subroutine exch(nmax,n,a,z,l,ls1,ls2)
		integer nmax,n,l,ls1,ls2
		double precision a(nmax,n),z(nmax,n)

		nmax     the first dimension of a, b and z
		n        the order of a, and z
	   *a        the matrix whose blocks are to be interchanged
	   *z        upon return this array is multiplied by the column
				 transformation zt.
		l        the position of the blocks
		ls1      the size of the first block
		ls2      the size of the second block

	auxiliary routines
		drot (blas)
		giv
		max abs (fortran)
	originator
		Delebecque f. and Steer s. INRIA adapted from exchqz (VanDooren)
*/

int dexchs(int _iMax, int _iLeadDim, double *_pdblIn, double *_pdblOut,
           int _iPos, int _iSize1, int _iSize2)
{
    int iLoop1		= 0; //i
    int iLoop2		= 0; //j
    int iIndex1 	= 0; //l1
    int iIndex2		= 0; //l2
    int iIndex3 	= 0; //l3
    int iIndex4 	= 0; //li
    int iIndex5		= 0; //lj
    int iIndex6		= 0; //ll

    //For Fortran compatibility
    int iZero		= 0;
    int iOne		= 1;
    int iTwo		= 2;
    int iThree		= 3;
    int iFour		= 4;


    double pdblTemp[3][4] = {0};
    double dblD		= 0;
    double dblE		= 0;
    double dblF		= 0;
    double dblG		= 0;
    double dblSa	= 0;
    double dblSb	= 0;

    double dblTemp1	= 0;
    double dblTemp2	= 0;
    double dblTemp3	= 0;

    iIndex1 = _iPos + 1;
    iIndex6	= _iSize1 + _iSize2;

    if (iIndex6 <= 2)
    {

        //interchange 1x1 and 1x1 blocks via an equivalence
        //transformation       a:=z'*a*z ,
        //where z is givens rotation

        dblF	= Max(dabss(_pdblIn[(iIndex1 - 1) + (iIndex1 - 1) * _iLeadDim]), 1);
        dblSa	= _pdblIn[(iIndex1 - 1) + (iIndex1 - 1) * _iLeadDim] / dblF;
        dblSb	= 1.0 / dblF;
        dblF	= dblSa - dblSb * _pdblIn[_iPos + _iPos * _iLeadDim];

        //construct the column transformation z
        dblG	= -dblSa * _pdblIn[_iPos + (iIndex1 - 1) * _iLeadDim];
        dgivs(dblF, dblG, &dblD, &dblE);

        dblTemp1 = -dblD;
        C2F(drot)(&iIndex1, &_pdblIn[_iPos * _iLeadDim], &iOne, &_pdblIn[(iIndex1 - 1) * _iLeadDim], &iOne, &dblE, &dblTemp1);
        C2F(drot)(&_iLeadDim, &_pdblOut[_iPos * _iLeadDim], &iOne, &_pdblOut[(iIndex1 - 1) * _iLeadDim], &iOne, &dblE, &dblTemp1);

        //construct the row transformation q
        dblTemp2 = _iLeadDim - _iPos + 1;
        C2F(drot)(&dblTemp2, &_pdblIn[_iPos + _iPos * _iLeadDim], &_iMax, &_pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim], &iOne, &dblE, &dblTemp1);
        _pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim]	= 0;
        return 0;
    }

    //interchange 1x1 and 2x2 blocks via an equivalence
    //transformation  a:=z2'*z1'*a*z1*z2 ,
    //where each zi is a givens rotation
    iIndex2	= _iPos + 2;

    if (_iSize1 != 2)
    {
        dblG	= Max(dabss(_pdblIn[_iPos + _iPos * _iLeadDim]), 1);
        //evaluate the pencil at the eigenvalue corresponding
        //to the 1x1 block
        dblSa	= _pdblIn[_iPos + _iPos * _iLeadDim] / dblG;
        dblSb	= 1 / dblG;
        for (iLoop1 = 0 ; iLoop1 < 2 ; iLoop1++)
        {
            iIndex5 = _iPos + iLoop1;
            for (iLoop2 = 0 ; iLoop2 < 3 ; iLoop2++)
            {
                iIndex4						= _iPos + iLoop2 - 1;
                pdblTemp[iLoop2][iLoop1]	= -dblSb * _pdblIn[iIndex4 + iIndex5 * _iLeadDim];
                if (iLoop1 == iLoop2)
                {
                    pdblTemp[iLoop2][iLoop1] += dblSa;
                }
            }//for
        }//for

        dgivs(pdblTemp[2][0], pdblTemp[2][1], &dblD, &dblE);
        dblTemp1 = -dblD;
        C2F(drot)(&iThree, &pdblTemp[0][0], &iOne, &pdblTemp[0][1], &iOne, &dblE, &dblTemp1);
        //perform the row transformation z1'
        dgivs(pdblTemp[0][0], pdblTemp[1][0], &dblD, &dblE);
        pdblTemp[1][1] = -pdblTemp[0][1] * dblE + pdblTemp[1][1] * dblD;
        dblTemp2 = _iLeadDim - _iPos + 1;
        C2F(drot)(&dblTemp2, &_pdblIn[_iPos + _iPos * _iLeadDim], &_iMax, &_pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim], &_iMax, &dblD, &dblE);
        //perform the column transformation z1
        C2F(drot)(&iIndex2, &_pdblIn[_iPos * _iLeadDim], &iOne, &_pdblIn[(iIndex1 - 1) * _iLeadDim], &iOne, &dblD, &dblE);
        C2F(drot)(&_iLeadDim, &_pdblOut[_iPos * _iLeadDim], &iOne, &_pdblOut[(iIndex1 - 1) * _iLeadDim], &iOne, &dblD, &dblE);
        //perform the row transformation z2'
        dgivs(pdblTemp[1][1], pdblTemp[2][1], &dblD, &dblE);
        C2F(drot)(&dblTemp2, &_pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim], &_iMax, &_pdblIn[(iIndex2 - 1) + _iPos * _iLeadDim], &_iMax, &dblD, &dblE);
        //perform the column transformation z2
        C2F(drot)(&iIndex2, &_pdblIn[(iIndex1 - 1) * _iLeadDim], &iOne, &_pdblIn[(iIndex2 - 1) * _iLeadDim], &iOne, &dblD, &dblE);
        C2F(drot)(&_iLeadDim, &_pdblOut[(iIndex1 - 1) * _iLeadDim], &iOne, &_pdblOut[(iIndex2 - 1) * _iLeadDim], &iOne, &dblD, &dblE);
        //put the neglectable elements equal to zero
        _pdblIn[(iIndex2 - 1) + _iPos * _iLeadDim] = 0;
        _pdblIn[(iIndex2 - 1) + (iIndex1 - 1) * _iLeadDim] = 0;
        return 0;
    }

    //interchange 2x2 and 1x1 blocks via an equivalence
    //transformation  a:=z2'*z1'*a*z1*z2 ,
    //where each zi is a givens rotation
    if (_iSize2 != 2)
    {
        dblG	= Max(dabss(_pdblIn[(iIndex2 - 1) + (iIndex2 - 1) * _iLeadDim]), 1);
        //evaluate the pencil at the eigenvalue corresponding
        //to the 1x1 block
        dblSa	= _pdblIn[(iIndex2 - 1) + (iIndex2 - 1) * _iLeadDim] / dblG;
        dblSb	= 1 / dblG;
        for (iLoop1 = 0 ; iLoop1 < 2 ; iLoop1++)
        {
            iIndex5 = _iPos + iLoop1 - 1;
            for (iLoop2 = 0 ; iLoop2 < 3 ; iLoop2++)
            {
                iIndex4						= _iPos + iLoop2 - 1;
                pdblTemp[iLoop1][iLoop2]	= -dblSb * _pdblIn[iIndex5 + iIndex4 * _iLeadDim];
                if (iLoop1 == iLoop2)
                {
                    pdblTemp[iLoop1][iLoop2] += dblSa;
                }
            }//for
        }//for

        dgivs(pdblTemp[0][0], pdblTemp[1][0], &dblD, &dblE);
        C2F(drot)(&iThree, &pdblTemp[0][0], &iThree, &pdblTemp[1][0], &iThree, &dblD, &dblE);
        //perform the row transformation z1
        dgivs(pdblTemp[1][1], pdblTemp[1][2], &dblD, &dblE);
        pdblTemp[0][1] = pdblTemp[0][1] * dblE - pdblTemp[0][2] * dblD;
        dblTemp1 = -dblD;
        C2F(drot)(&iIndex2, &_pdblIn[(iIndex1 - 1) * _iLeadDim], &iOne, &_pdblIn[(iIndex2 - 1) * _iLeadDim], &iOne, &dblE, &dblTemp1);
        C2F(drot)(&_iLeadDim, &_pdblOut[(iIndex1 - 1) * _iLeadDim], &iOne, &_pdblOut[(iIndex2 - 1) * _iLeadDim], &iOne, &dblE, &dblTemp1);
        //perform the column transformation z1'
        dblTemp2 = _iLeadDim - _iPos + 1;
        C2F(drot)(&dblTemp2, &_pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim], &_iMax, &_pdblIn[(iIndex2 - 1) + _iPos * _iLeadDim], &_iMax, &dblE, &dblTemp1);
        //perform the row transformation z2
        dgivs(pdblTemp[0][0], pdblTemp[0][1], &dblD, &dblE);
        C2F(drot)(&dblTemp2, &_pdblIn[_iPos * _iLeadDim], &iOne, &_pdblIn[(iIndex1 - 1) * _iLeadDim], &iOne, &dblE, &dblTemp1);
        C2F(drot)(&_iLeadDim, &_pdblOut[_iPos * _iLeadDim], &iOne, &_pdblOut[(iIndex1 - 1) * _iLeadDim], &iOne, &dblE, &dblTemp1);
        //perform the column transformation z2'
        C2F(drot)(&dblTemp2, &_pdblIn[_iPos + _iPos * _iLeadDim], &_iMax, &_pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim], &iOne, &dblE, &dblTemp1);
        //put the neglectable elements equal to zero
        _pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim] = 0;
        _pdblIn[(iIndex2 - 1) + _iPos * _iLeadDim] = 0;
        return 0;
    }

    iIndex3	= _iPos + 3;
    dblD	=	_pdblIn[(iIndex2 - 1) + (iIndex2 - 1) * _iLeadDim] *
                _pdblIn[(iIndex3 - 1) + (iIndex3 - 1) * _iLeadDim] -
                _pdblIn[(iIndex2 - 1) + (iIndex3 - 1) * _iLeadDim] *
                _pdblIn[(iIndex3 - 1) + (iIndex2 - 1) * _iLeadDim];

    dblE	=	_pdblIn[(iIndex2 - 1) + (iIndex2 - 1) * _iLeadDim] +
                _pdblIn[(iIndex3 - 1) + (iIndex3 - 1) * _iLeadDim];

    ddmmuls(	&_pdblIn[_iPos + _iPos * _iLeadDim], _iMax,
                &_pdblIn[_iPos + _iPos * _iLeadDim], _iMax,
                pdblTemp[0], 3, 2, 4, 4);

    for (iLoop1 = 0 ; iLoop1 < 2 ; iLoop1++)
    {
        pdblTemp[iLoop1][iLoop1] += dblD;
        for (iLoop2 = 0 ; iLoop2 < 4 ; iLoop2++)
        {
            pdblTemp[iLoop1][iLoop2] = pdblTemp[iLoop1][iLoop2] - dblE *
                                       _pdblIn[(_iPos - 1 + iLoop1) + (_iPos - 1 + iLoop2) * _iLeadDim];
        }
    }

    //g0
    dgivs(pdblTemp[0][0], pdblTemp[1][0], &dblD, &dblE);
    C2F(drot)(&iFour, &pdblTemp[0][0], &iThree, &pdblTemp[1][0], &iThree, &dblD, &dblE);

    //z1
    dgivs(pdblTemp[1][3], pdblTemp[1][2], &dblD, &dblE);
    C2F(drot)(&iTwo, &pdblTemp[0][3], &iOne, &pdblTemp[0][2], &iOne, &dblD, &dblE);
    C2F(drot)(&iIndex3, &_pdblIn[(iIndex3 - 1) * _iLeadDim], &iOne, &_pdblIn[(iIndex2 - 1) * _iLeadDim], &iOne, &dblD, &dblE);
    C2F(drot)(&_iLeadDim, &_pdblOut[(iIndex3 - 1) * _iLeadDim], &iOne, &_pdblOut[(iIndex2 - 1) * _iLeadDim], &iOne, &dblD, &dblE);

    //z1'
    C2F(drot)(&dblTemp2, &_pdblIn[(iIndex3 - 1) + _iPos * _iLeadDim], &_iMax, &_pdblIn[(iIndex2 - 1) + _iPos * _iLeadDim], &iOne, &dblD, &dblE);

    //z2
    dgivs(pdblTemp[1][3], pdblTemp[1][1], &dblD, &dblE);
    C2F(drot)(&iTwo, &pdblTemp[0][3], &iOne, &pdblTemp[0][1], &iOne, &dblD, &dblE);
    C2F(drot)(&iIndex3, &_pdblIn[(iIndex3 - 1) * _iLeadDim], &iOne, &_pdblIn[(iIndex1 - 1) * _iLeadDim], &iOne, &dblD, &dblE);
    C2F(drot)(&_iLeadDim, &_pdblOut[(iIndex3 - 1) * _iLeadDim], &iOne, &_pdblOut[(iIndex1 - 1) * _iLeadDim], &iOne, &dblD, &dblE);

    //z2'
    pdblTemp[1][3] *=  dblD;
    C2F(drot)(&dblTemp2, &_pdblIn[(iIndex3 - 1) + _iPos * _iLeadDim], &_iMax, &_pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim], &_iMax, &dblD, &dblE);

    //z3
    dgivs(pdblTemp[0][2], pdblTemp[0][1], &dblD, &dblE);
    C2F(drot)(&iOne, &pdblTemp[0][2], &iOne, &pdblTemp[0][1], &iOne, &dblD, &dblE);
    C2F(drot)(&iIndex3, &_pdblIn[(iIndex2 - 1) * _iLeadDim], &iOne, &_pdblIn[(iIndex1 - 1) * _iLeadDim], &iOne, &dblD, &dblE);
    C2F(drot)(&_iLeadDim, &_pdblOut[(iIndex2 - 1) * _iLeadDim], &iOne, &_pdblOut[(iIndex1 - 1) * _iLeadDim], &iOne, &dblD, &dblE);

    //z3'
    pdblTemp[1][3] *=  dblD;
    C2F(drot)(&dblTemp2, &_pdblIn[(iIndex2 - 1) + _iPos * _iLeadDim], &_iMax, &_pdblIn[(iIndex1 - 1) + _iPos * _iLeadDim], &_iMax, &dblD, &dblE);

    //z4
    dgivs(pdblTemp[0][2], pdblTemp[0][0], &dblD, &dblE);
    C2F(drot)(&iIndex3, &_pdblIn[(iIndex2 - 1) * _iLeadDim], &iOne, &_pdblIn[_iPos * _iLeadDim], &iOne, &dblD, &dblE);
    C2F(drot)(&_iLeadDim, &_pdblOut[(iIndex2 - 1) * _iLeadDim], &iOne, &_pdblOut[_iPos * _iLeadDim], &iOne, &dblD, &dblE);

    //z4'
    C2F(drot)(&dblTemp2, &_pdblIn[(iIndex2 - 1) + _iPos * _iLeadDim], &_iMax, &_pdblIn[_iPos + _iPos * _iLeadDim], &_iMax, &dblD, &dblE);

    //zeroes negligible elements
    _pdblIn[(iIndex2 - 1) + _iPos * _iLeadDim]			= 0;
    _pdblIn[(iIndex3 - 1) + _iPos * _iLeadDim]			= 0;
    _pdblIn[(iIndex2 - 1) + (iIndex1 - 1) * _iLeadDim]	= 0;
    _pdblIn[(iIndex3 - 1) + (iIndex1 - 1) * _iLeadDim]	= 0;

    return 0;
}

/*
	purpose
		 this routine constructs the givens transformation

				   ( sc  ss )
			   g = (        ),   sc**2+ss**2 = 1. ,
				   (-ss  sc )

		 which zeros the second entry of the 2-vector (sa,sb)**t
		 this routine is a modification of the blas routine srotg
		 (algorithm 539) in order to leave the arguments sa and sb
		 unchanged

	syntax

		subroutine giv(sa,sb,sc,ss)
		double precision sa,sb,sc,ss
	auxiliary routines
		sqrt abs (fortran)
*/
int dgivs(double _dblA, double _dblB, double *_pdblSC, double *_pdblSS)
{
    double dblU = 0;
    double dblR = 0;
    double dblV	= 0;
    if (dabss(_dblA) > dabss(_dblB))
    {
        dblU		= _dblA + _dblA;
        dblV		= _dblB / dblU;
        dblR		= dsqrts(0.25 + dblV * dblV) * dblU;
        *_pdblSC	= _dblA / dblR;
        *_pdblSS	= dblV * (*_pdblSC + *_pdblSC);
    }
    else if (_dblB != 0)
    {
        dblU		= _dblB + _dblB;
        dblV		= _dblA / dblU;
        dblR		= dsqrts(0.25 + dblV * dblV) * dblU;
        *_pdblSS	= _dblB / dblR;
        *_pdblSC	= dblV * (*_pdblSS + *_pdblSS);
    }
    else
    {
        //dblB == 0
        *_pdblSC	= 0;
        *_pdblSS	= 0;
    }
    return 0;
}

/*
purpose

    given the upper hessenberg matrix a with a 2x2 block
    starting at a(l,l), split determines if the
    corresponding eigenvalues are real or complex, if they
    are real, a rotation is determined that reduces the
    block to upper triangular form with the eigenvalue
    of largest absolute value appearing first.  the
    rotation is accumulated in v. the eigenvalues (real
    or complex) are returned in e1 and e2.
syntax

    subroutine split(a, v, n, l, e1, e2, na, nv)

    double precision a,v,e1,e2
    integer n,l,na,nv
    dimension a(na,n),v(nv,n)

    starred parameters are  altered by the subroutine

       *a        the upper hessenberg matrix whose 2x2
                 block is to be dsplit.
       *v        the array in which the dsplitting trans-
                 formation is to be accumulated.
        n        the order of the matrix a.
        l        the position of the 2x2 block.
       *e1       on return if the eigenvalues are complex
       *e2       e1 contains their common real part and
                 e2 contains the positive imaginary part.
                 if the eigenvalues are real. e1 contains
                 the one largest in absolute value and f2
                 contains the other one.
       na        the first dimension of the array a.
       nv        the first dimension of the array v.
auxiliary routines
    abs sqrt (fortran)
*/

int dsplits(double *_pdblVal, double *_pdblSplit, int _iSize, int _iPos, double *_pdblE1, double *_pdblE2, int _iLeadDimVal, int _iLeadDimSplit)
{
    double dblZero	= 0;
    double dblTwo	= 2;
    double dblP = 0, dblQ = 0, dblR = 0, dblS = 0, dblT = 0, dblU = 0;
    double dblW = 0, dblX = 0, dblY = 0, dblZ = 0;

    int iLoop1	= 0;
    int iLoop2	= 0;
    int iIndex	= 0;

    iIndex	= _iPos + 1;

    dblX	=	_pdblVal[(iIndex - 1) + (iIndex - 1) * _iLeadDimVal];
    dblY	=	_pdblVal[_iPos + _iPos * _iLeadDimVal];
    dblW	=	_pdblVal[_iPos + (iIndex - 1) * _iLeadDimVal] *
                _pdblVal[(iIndex - 1) + _iPos * _iLeadDimVal];
    dblP	= (dblY / dblX) / dblTwo;
    dblQ	= dblP * dblP + dblW;

    if (dblQ < 0)
    {
        //complex eigenvalue.
        *_pdblE1	= dblP + dblX;
        *_pdblE2	= dsqrts(-dblQ);
        return 0;
    }

    //L10
    //two real eigenvalues. set up transformation
    dblZ	= dsqrts(dblQ);
    if (dblP < dblZero)
    {
        dblZ	= dblP + dblZ;
    }
    else
    {
        dblZ	= dblP - dblZ;
    }

    if (dblZ == dblZero)
    {
        dblR	= dblZero;
    }
    else
    {
        dblR	= -dblW / dblZ;
    }

    if (dabss(dblX + dblZ) >= dabss(dblX + dblR))
    {
        dblZ	= dblR;
    }

    dblY	= dblY - dblX - dblZ;
    dblX	= -dblZ;
    dblT	= _pdblVal[_iPos + (iIndex - 1) * _iLeadDimVal];
    dblU	= _pdblVal[(iIndex - 1) + _iPos * _iLeadDimVal];

    if (dabss(dblY) + dabss(dblU) <= dabss(dblT) + dabss(dblX))
    {
        dblQ	= dblX;
        dblP	= dblT;
    }
    else
    {
        dblQ	= dblU;
        dblP	= dblY;
    }

    dblR	= dsqrts(dblP * dblP + dblQ * dblQ);
    if (dblR <= dblZero)
    {
        *_pdblE1	= _pdblVal[_iPos + _iPos * _iLeadDimVal];
        *_pdblE2	= _pdblVal[(iIndex - 1) + (iIndex - 1) * _iLeadDimVal];
        _pdblVal[(iIndex - 1) + _iPos * _iLeadDimVal]	= dblZero;
        return 0;
    }

    dblP	/= dblR;
    dblQ	/= dblR;

    //premultiply
    for (iLoop1 = _iPos ; iLoop1 < _iSize ; iLoop1++)
    {
        dblZ	= _pdblVal[_iPos + iLoop1 * _iLeadDimVal];
        _pdblVal[_iPos + iLoop1 * _iLeadDimVal]			= dblP * dblZ +
                dblQ * _pdblVal[(iIndex - 1) + iLoop1 * _iLeadDimVal];
        _pdblVal[(iIndex - 1) + iLoop1 * _iLeadDimVal]	= dblP *
                _pdblVal[(iIndex - 1) + iLoop1 * _iLeadDimVal] - dblQ * dblZ;
    }

    //postmultiply
    for (iLoop1 = 0 ; iLoop1 < iIndex ; iLoop1++)
    {
        dblZ	= _pdblVal[iLoop1 + _iPos * _iLeadDimVal];
        _pdblVal[iLoop1 + _iPos * _iLeadDimVal]			= dblP * dblZ +
                dblQ * _pdblVal[iLoop1 + (iIndex - 1) * _iLeadDimVal];
        _pdblVal[iLoop1 + (iIndex - 1) * _iLeadDimVal]	= dblP *
                _pdblVal[iLoop1 + (iIndex - 1) * _iLeadDimVal] - dblQ * dblZ;
    }

    //accumulate the transformation in v
    for (iLoop1 = 0 ; iLoop1 < _iSize ; iLoop1++)
    {
        dblZ	= _pdblSplit[iLoop1 + _iPos * _iLeadDimVal];
        _pdblSplit[iLoop1 + _iPos * _iLeadDimVal]			= dblP * dblZ +
                dblQ * _pdblSplit[iLoop1 + (iIndex - 1) * _iLeadDimVal];
        _pdblSplit[iLoop1 + (iIndex - 1) * _iLeadDimVal]	= dblP *
                _pdblSplit[iLoop1 + (iIndex - 1) * _iLeadDimVal] - dblQ * dblZ;
    }

    _pdblVal[(iIndex - 1) + _iPos * _iLeadDimVal]	= dblZero;
    *_pdblE1	= _pdblVal[_iPos + _iPos * _iLeadDimVal];
    *_pdblE2	= _pdblVal[(iIndex - 1) + (iIndex - 1) * _iLeadDimVal];
    return 0;
}


/*
C2F(dgemm)("n", "n", &iRows1, &iCols2, &iCols1, &dblOne, pReal1 , &iRows1 , pReal2, &iRows2, &dblZero, pReturnReal ,&iRows1);
if(iComplex1 == 1)
	C2F(dgemm)("n", "n", &iRows1, &iCols2, &iCols1, &dblOne, pImg1 , &iRows1 , pReal2, &iRows2, &dblZero, pReturnImg ,&iRows1);
if(iComplex2 == 1)
	C2F(dgemm)("n", "n", &iRows1, &iCols2, &iCols1, &dblOne, pReal1 , &iRows1 , pImg2, &iRows2, &dblZero, pReturnImg ,&iRows1);
*/

int dexpms2(double *_pdblReal, double *_pdblReturnReal, int _iLeadDim)
{
    int iRet = zexpms2(_pdblReal, NULL, _pdblReturnReal, NULL, _iLeadDim);
    return iRet;
}

int zexpms2(double *_pdblReal, double *_pdblImg, double *_pdblReturnReal, double *_pdblReturnImg, int _iLeadDim)
{
    double dblRcond = 0;
    int iRet				= 0;
    int iIndex1			= 0;
    int iMax				= 0;
    int iFlag				= 0;
    int iLoop1			= 0;
    int iSquare			= 0;
    int iOne				= 1;

    int iComplex = 0;

    double dblZero	= 0;
    double dblOne	= 1;

    double dblExp	= 0;
    double dblS		= 0;
    double dblS2	= 0;
    double dblCst	= 0.5;

    double *pdblMatrixRealA		= NULL;//A'
    double *pdblMatrixRealX		= NULL;//X
    double *pdblMatrixRealD		= NULL;//D
    double *pdblMatrixRealcX	= NULL;//cX
    double *pdblMatrixRealcA	= NULL;//cX
    double *pdblMatrixRealEye	= NULL;//Eye
    double *pdblMatrixRealTemp	= NULL;//Temp
    double *pdblMatrixRealTemp2	= NULL;//Temp2

    double *pdblMatrixImgA		= NULL;//A'
    double *pdblMatrixImgX		= NULL;//X
    double *pdblMatrixImgD		= NULL;//D
    double *pdblMatrixImgcX		= NULL;//cX
    double *pdblMatrixImgcA		= NULL;//cX
    double *pdblMatrixImgEye	= NULL;//Eye
    double *pdblMatrixImgTemp	= NULL;//Temp
    double *pdblMatrixImgTemp2	= NULL;//Temp2

    if (_pdblImg != NULL)
    {
        iComplex = 1;
    }

    iSquare = _iLeadDim * _iLeadDim;

    pdblMatrixRealA			= (double*)malloc(sizeof(double) * iSquare);
    pdblMatrixRealX			= (double*)malloc(sizeof(double) * iSquare);
    pdblMatrixRealD			= (double*)malloc(sizeof(double) * iSquare);
    pdblMatrixRealcX		= (double*)malloc(sizeof(double) * iSquare);
    pdblMatrixRealcA		= (double*)malloc(sizeof(double) * iSquare);
    pdblMatrixRealEye		= (double*)malloc(sizeof(double) * iSquare);
    pdblMatrixRealTemp		= (double*)malloc(sizeof(double) * iSquare);
    pdblMatrixRealTemp2		= (double*)malloc(sizeof(double) * iSquare);

    if (iComplex)
    {
        pdblMatrixImgA			= (double*)malloc(sizeof(double) * iSquare);
        pdblMatrixImgX			= (double*)malloc(sizeof(double) * iSquare);
        pdblMatrixImgD			= (double*)malloc(sizeof(double) * iSquare);
        pdblMatrixImgcX			= (double*)malloc(sizeof(double) * iSquare);
        pdblMatrixImgcA			= (double*)malloc(sizeof(double) * iSquare);
        pdblMatrixImgEye		= (double*)malloc(sizeof(double) * iSquare);
        pdblMatrixImgTemp		= (double*)malloc(sizeof(double) * iSquare);
        pdblMatrixImgTemp2		= (double*)malloc(sizeof(double) * iSquare);

        memset(pdblMatrixImgEye, 0x00, sizeof(double) * iSquare);
    }


    // Scale A by power of 2 so that its norm is < 1/2 .
    dfrexps(dblGetMatrixInfiniteNorm(_pdblReal, _pdblImg, _iLeadDim, _iLeadDim), &dblExp);
    dblS	= Max(0, dblExp + 1);
    dblS2	= pow(2, dblS);

    //A = A./2^s
    if (iComplex)
    {
        iRightDivisionComplexMatrixByRealMatrix(
            _pdblReal,			_pdblImg	,		1,
            &dblS2,				0,
            pdblMatrixRealA,	pdblMatrixImgA,	1,	iSquare);
    }
    else
    {
        iRightDivisionRealMatrixByRealMatrix(
            _pdblReal,			1,
            &dblS2,				0,
            pdblMatrixRealA,	1,	iSquare);
    }

    // Pade approximation for exp(A)
    //X = A
    C2F(dcopy)(&iSquare, pdblMatrixRealA, &iOne, pdblMatrixRealX, &iOne );
    if (iComplex)
    {
        C2F(dcopy)(&iSquare, pdblMatrixImgA, &iOne, pdblMatrixImgX, &iOne );
    }


    deyes(pdblMatrixRealEye, _iLeadDim, _iLeadDim);


    //cA = A * c
    if (iComplex)
        iMultiRealScalarByComplexMatrix(
            dblCst,
            pdblMatrixRealA, pdblMatrixImgA, _iLeadDim, _iLeadDim,
            pdblMatrixRealcA, pdblMatrixImgcA);
    else
        iMultiRealScalarByRealMatrix(
            dblCst,
            pdblMatrixRealA, _iLeadDim, _iLeadDim,
            pdblMatrixRealcA);
    /*
    	C2F(dcopy)(&iSquare, pdblMatrixRealA, &iOne, pdblMatrixRealcA, &iOne);
    	C2F(dscal)(&iSquare, &dblCst, pdblMatrixRealcA, &iOne);
    */

    //E = Eye + cA
    vDadd(iSquare, pdblMatrixRealEye, pdblMatrixRealcA, 1, 1, _pdblReturnReal);
    if (iComplex)
    {
        vDadd(iSquare, pdblMatrixImgEye, pdblMatrixImgcA, 1, 1, _pdblReturnImg);
    }

    //D = Eye - cA
    vDless(iSquare, pdblMatrixRealEye, pdblMatrixRealcA, 1, 1, pdblMatrixRealD);
    if (iComplex)
    {
        vDless(iSquare, pdblMatrixImgEye, pdblMatrixImgcA, 1, 1, pdblMatrixImgD);
    }

    iMax	= 6;
    iFlag	= 1;

    for (iLoop1 = 2 ; iLoop1 <= iMax ; iLoop1++)
    {
        dblCst	= dblCst * (iMax - iLoop1 + 1 ) / (iLoop1 * (2 * iMax - iLoop1 + 1));

        //Temp = X
        C2F(dcopy)(&iSquare, pdblMatrixRealX, &iOne, pdblMatrixRealTemp, &iOne);
        if (iComplex)
        {
            C2F(dcopy)(&iSquare, pdblMatrixImgX, &iOne, pdblMatrixImgTemp, &iOne);
        }
        /*		for(iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++)
        			pdblMatrixRealTemp[iIndex1]	= pdblMatrixRealX[iIndex1];
        */
        //X = A * Temp;
        if (iComplex)
        {
            iMultiComplexMatrixByComplexMatrix(
                pdblMatrixRealA, pdblMatrixImgA, _iLeadDim, _iLeadDim,
                pdblMatrixRealTemp, pdblMatrixImgTemp, _iLeadDim, _iLeadDim,
                pdblMatrixRealX, pdblMatrixImgX);
        }
        else
        {
            iMultiRealMatrixByRealMatrix(
                pdblMatrixRealA, _iLeadDim, _iLeadDim,
                pdblMatrixRealTemp, _iLeadDim, _iLeadDim,
                pdblMatrixRealX);
        }
        /*		C2F(dgemm)("n", "n", &_iLeadDim, &_iLeadDim, &_iLeadDim, &dblOne,
        			pdblMatrixRealA, &_iLeadDim ,
        			pdblMatrixRealTemp, &_iLeadDim, &dblZero,
        			pdblMatrixRealX ,&_iLeadDim);
        */
        //cX = c * X
        if (iComplex)
            iMultiRealScalarByComplexMatrix(
                dblCst,
                pdblMatrixRealX, pdblMatrixImgX, _iLeadDim, _iLeadDim,
                pdblMatrixRealcX, pdblMatrixImgcX);
        else
            iMultiRealScalarByRealMatrix(
                dblCst,
                pdblMatrixRealX, _iLeadDim, _iLeadDim,
                pdblMatrixRealcX);
        /*		C2F(dcopy)(&iSquare, pdblMatrixRealX, &iOne, pdblMatrixRealcX, &iOne);
        		C2F(dscal)(&iSquare, &dblCst, pdblMatrixRealcX, &iOne);
        */

        //E = E + cX
        vDadd(iSquare, _pdblReturnReal, pdblMatrixRealcX, 1, 1, _pdblReturnReal);
        if (iComplex)
        {
            vDadd(iSquare, _pdblReturnImg, pdblMatrixImgcX, 1, 1, _pdblReturnImg);
        }


        if (iFlag == 1) //D = D + cX
        {
            vDadd(iSquare, pdblMatrixRealD, pdblMatrixRealcX, 1, 1, pdblMatrixRealD);
            if (iComplex)
            {
                vDadd(iSquare, pdblMatrixImgD, pdblMatrixImgcX, 1, 1, pdblMatrixImgD);
            }
        }
        else //D = D - cX
        {
            vDless(iSquare, pdblMatrixRealD, pdblMatrixRealcX, 1, 1, pdblMatrixRealD);
            if (iComplex)
            {
                vDless(iSquare, pdblMatrixImgD, pdblMatrixImgcX, 1, 1, pdblMatrixImgD);
            }
        }

        //Toggle iFlag
        iFlag = !iFlag;
    }

    //Temp = E
    C2F(dcopy)(&iSquare, _pdblReturnReal, &iOne, pdblMatrixRealTemp, &iOne);
    if (iComplex)
    {
        C2F(dcopy)(&iSquare, _pdblReturnImg, &iOne, pdblMatrixImgTemp, &iOne);
        /*	for(iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++)
        		pdblMatrixRealTemp[iIndex1]	= _pdblReturnReal[iIndex1];
        */
    }

    //E = D\E
    if (iComplex)
    {
        iRet = iLeftDivisionOfComplexMatrix(
                   pdblMatrixRealD,	pdblMatrixImgD,		_iLeadDim, _iLeadDim,
                   pdblMatrixRealTemp,	pdblMatrixImgTemp,	_iLeadDim, _iLeadDim,
                   _pdblReturnReal,	_pdblReturnImg,		_iLeadDim, _iLeadDim, &dblRcond);
    }
    else
    {
        iRet = iLeftDivisionOfRealMatrix(
                   pdblMatrixRealD,	_iLeadDim, _iLeadDim,
                   pdblMatrixRealTemp,	_iLeadDim, _iLeadDim,
                   _pdblReturnReal,	_iLeadDim, _iLeadDim, &dblRcond);
    }

    if (iRet < 0)
    {
        switch (iRet)
        {
            case -1 :
                //Scilab 6
                //sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
                //Msgs(5,1);
                break;
            case -2 :
                //Scilab 6
                //Msgs(9, (int)dblRcond);
                break;
            default :
                break;
        }
    }
    // Undo scaling by repeated squaring
    for (iLoop1 = 0 ; iLoop1 < dblS ; iLoop1++)
    {
        //Temp = E
        C2F(dcopy)(&iSquare, _pdblReturnReal, &iOne, pdblMatrixRealTemp, &iOne);
        //Temp2 = E
        C2F(dcopy)(&iSquare, _pdblReturnReal, &iOne, pdblMatrixRealTemp2, &iOne);
        if (iComplex)
        {
            //Temp = E
            C2F(dcopy)(&iSquare, _pdblReturnImg, &iOne, pdblMatrixImgTemp, &iOne);
            //Temp2 = E
            C2F(dcopy)(&iSquare, _pdblReturnImg, &iOne, pdblMatrixImgTemp2, &iOne);
        }


        /*		for(iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++)
        			pdblMatrixRealTemp[iIndex1]		= _pdblReturnReal[iIndex1];
        */
        /*		for(iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++)
        			pdblMatrixRealTemp2[iIndex1]	= _pdblReturnReal[iIndex1];
        */
        // E = E*E
        if (iComplex)
            iMultiComplexMatrixByComplexMatrix(
                pdblMatrixRealTemp,		pdblMatrixImgTemp,	_iLeadDim, _iLeadDim,
                pdblMatrixRealTemp2,	pdblMatrixImgTemp2,	_iLeadDim, _iLeadDim,
                _pdblReturnReal,		_pdblReturnImg);
        else
            iMultiRealMatrixByRealMatrix(
                pdblMatrixRealTemp,		_iLeadDim, _iLeadDim,
                pdblMatrixRealTemp2,	_iLeadDim, _iLeadDim,
                _pdblReturnReal);
        /*
        		C2F(dgemm)("n", "n", &_iLeadDim, &_iLeadDim, &_iLeadDim, &dblOne,
        			pdblMatrixRealTemp,		&_iLeadDim ,
        			pdblMatrixRealTemp2,	&_iLeadDim, &dblZero,
        			_pdblReturnReal ,		&_iLeadDim);
        */
    }

    free(pdblMatrixRealA);
    free(pdblMatrixRealX);
    free(pdblMatrixRealD);
    free(pdblMatrixRealcX);
    free(pdblMatrixRealcA);
    free(pdblMatrixRealEye);
    free(pdblMatrixRealTemp);
    free(pdblMatrixRealTemp2);

    if (iComplex)
    {
        free(pdblMatrixImgA);
        free(pdblMatrixImgX);
        free(pdblMatrixImgD);
        free(pdblMatrixImgcX);
        free(pdblMatrixImgcA);
        free(pdblMatrixImgEye);
        free(pdblMatrixImgTemp);
        free(pdblMatrixImgTemp2);
    }

    return 0;
}

double dblGetMatrixInfiniteNorm(double *_pdblReal, double *_pdblImg, int _iRows, int _iCols)
{
    int iIndex1 = 0, iIndex2 = 0;
    double dblTemp = 0;
    double dblRef = 0;

    if (_pdblImg == NULL)
    {
        for (iIndex1 = 0 ; iIndex1 < _iRows ; iIndex1++)
        {
            dblTemp = 0;
            for (iIndex2 = 0 ; iIndex2 < _iCols ; iIndex2++)
            {
                dblTemp += _pdblReal[iIndex1 + iIndex2 * _iRows];
            }
            if (dblTemp > dblRef)
            {
                dblRef = dblTemp;
            }
        }
    }
    else
    {
        for (iIndex1 = 0 ; iIndex1 < _iRows ; iIndex1++)
        {
            dblTemp = 0;
            for (iIndex2 = 0 ; iIndex2 < _iCols ; iIndex2++)
            {
                dblTemp += dpythags(_pdblReal[iIndex1 + iIndex2 * _iRows], _pdblImg[iIndex1 + iIndex2 * _iRows]);
            }
            if (dblTemp > dblRef)
            {
                dblRef = dblTemp;
            }
        }
    }
    return dblRef;
}
