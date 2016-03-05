/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
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
#include "basic_functions.h"
#include "string.h"
/*
PURPOSE
   val(0..n) being an array (with strict increasing order and n >=1)
   representing intervals, this routine, by the mean of a
   dichotomic search, computes :

      a/ for each X(i) its interval number indX(i) :
                indX(i) = j if  X(i) in (val(j-1), val(j)]
                        = 1 if  X(i) = val(0)
                        = 0 if  X(i) is not in [val(0),val(n)]

      b/ the number of points falling in the interval j :

         occ(j) = # { X(i) such that X(i) in (val(j-1), val(j)] } for j>1
    and  occ(1) = # { X(i) such that X(i) in [val(0), val(1)] }

PARAMETERS
   inputs :
      m         integer
      X(1..m)   double float array
      n         integer
      val(0..n) double float array (val(0) < val(1) < ....)
   outputs
      indX(1..m) integer array
      occ(1..n)  integer array
      info       integer (number of X(i) not in [val(0), val(n)])

AUTHOR
   Bruno Pincon

TRANSLATION TO C
	Antoine Elias

	PARAMETERS
	Inputs :
		m			-> _iNbElemX
		X(1..m)		-> _pdblX[_iNbElemX]
		n			-> _iNbElemVal
		val(0..n)	-> _pdblVal[_iNbElemVal]

	Outputs :
		indX(1..m)	-> _pdblInd[_iNbElemX]
		occ(1..n)	-> _pdblOcc[_iNbElemVal]
		info		-> _pdblInfo
*/

void vDsearchC(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo)
{
    int iLoop = 0;

    if (_pdblOcc != NULL)
    {
        memset(_pdblOcc, 0x00, _iNbElemVal * sizeof(double));
    }

    if (_pdblInfo != NULL)
    {
        *_pdblInfo	= 0;
    }

    for (iLoop = 0 ; iLoop < _iNbElemX ; iLoop++)
    {
        if (_pdblVal[0] <= _pdblX[iLoop] && _pdblX[iLoop] <= _pdblVal[_iNbElemVal])
        {
            int iIndex = 0;
            int iIndex1 = 0;
            int iIndex2 = _iNbElemVal + 1;
            while (iIndex2 - iIndex1 > 1)
            {
                iIndex = (iIndex1 + iIndex2) / 2;
                if ( _pdblX[iLoop] <= _pdblVal[iIndex])
                {
                    iIndex2 = iIndex;
                }
                else
                {
                    iIndex1 = iIndex;
                }
            }

            _pdblInd[iLoop]	= iIndex2;
            if (_pdblOcc != NULL)
            {
                _pdblOcc[iIndex2 - 1]++;
            }
        }
        else
        {
            if (_pdblInfo != NULL)
            {
                (*_pdblInfo)++;
            }
            _pdblInd[iLoop] = 0;
        }
    }
}

/*
 PURPOSE
    val(1..n) being a strictly increasing array, this
    routines by the mean of a dichotomic search computes :

    a/ the number of occurences (occ(j)) of each value val(j)
       in the array X :

          occ(j) = #{ X(i) such that X(i) = val(j) }

    b/ the array indX :  if X(i) = val(j) then indX(i) = j
       (if X(i) is not in val then indX(i) = 0)

 PARAMETERS
    inputs :
       m         integer
       X(1..m)   double float array
       n         integer
       val(1..n) double float array (must be in a strict increasing order)
    outputs :
       occ(1..n)  integer array
       indX(1..m) integer array
       info       integer  (number of X(i) which are not in val(1..n))

 AUTHOR
    Bruno Pincon

TRANSLATION TO C
	Antoine Elias

	PARAMETERS
	Inputs :
		m			-> _iNbElemX
		X(1..m)		-> _pdblX[_iNbElemX]
		n			-> _iNbElemVal
		val(1..n)	-> _pdblVal[_iNbElemVal]

	Outputs :
		occ(1..n)	-> _pdblOcc[_iNbElemVal]
		indX(1..m)	-> _pdblInd[_iNbElemX]
		info		-> _pdblInfo
*/

void vDsearchD(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo)
{
    int iLoop = 0;

    if (_pdblOcc != NULL)
    {
        memset(_pdblOcc, 0x00, _iNbElemVal * sizeof(double));
    }

    if (_pdblInfo != NULL)
    {
        *_pdblInfo	= 0;
    }

    for (iLoop = 0 ; iLoop < _iNbElemX ; iLoop++)
    {
        if (_pdblVal[0] <= _pdblX[iLoop] && _pdblX[iLoop] <= _pdblVal[_iNbElemVal - 1])
        {
            int iIndex = 0;
            int iIndex1 = 0;
            int iIndex2 = _iNbElemVal - 1;
            while (iIndex2 - iIndex1 > 1)
            {
                iIndex = (iIndex1 + iIndex2) / 2;
                if ( _pdblX[iLoop] < _pdblVal[iIndex])
                {
                    iIndex2 = iIndex;
                }
                else
                {
                    iIndex1 = iIndex;
                }
            }
            if (_pdblX[iLoop] == _pdblVal[iIndex1])
            {
                if (_pdblOcc != NULL)
                {
                    _pdblOcc[iIndex1]++;
                }
                _pdblInd[iLoop]	= iIndex1 + 1;
            }
            else if (_pdblX[iLoop] == _pdblVal[iIndex2])
            {
                if (_pdblOcc != NULL)
                {
                    _pdblOcc[iIndex2]++;
                }
                _pdblInd[iLoop]	= iIndex2 + 1;
            }
            else
            {
                if (_pdblInfo != NULL)
                {
                    (*_pdblInfo)++;
                }
                _pdblInd[iLoop] = 0;
            }
        }
        else
        {
            if (_pdblInfo != NULL)
            {
                (*_pdblInfo)++;
            }
            _pdblInd[iLoop] = 0;
        }
    }
}
