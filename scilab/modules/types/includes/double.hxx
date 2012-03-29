/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

// This code is separated in double.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __DOUBLE_HXX__
#define __DOUBLE_HXX__

#include "arrayof.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP Double : public ArrayOf<double>
    {
    public :
        virtual						~Double();

                                    Double(double _dblReal);
                                    Double(double _dblReal, double _dblImg);
                                    Double(int _iRows, int _iCols, bool _bComplex = false);
                                    Double(int _iRows, int _iCols, double **_pdblReal);
                                    Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
                        	        Double(int _iDims, int* _piDims, bool _bComplex = false);

        static Double*              Empty();

        /*data management*/
        double*                     getReal() const;
        double                      getReal(int _iRows, int _iCols);
        bool                        setInt(int* _piReal); //to translate int to double matrix

        /*zero or one set filler*/
        bool                        setZeros();
        bool                        setOnes();

        /*Config management*/
        void                        whoAmI();
        bool                        isEmpty();

        InternalType*               clone();
        bool                        fillFromCol(int _iCols, Double *_poSource);
        bool                        fillFromRow(int _iRows, Double *_poSource);
        bool                        append(int _iRows, int _iCols, InternalType* _poSource);

	    //bool                        append(int _iRows, int _iCols, Double *_poSource);

        bool                        operator==(const InternalType& it);
        bool                        operator!=(const InternalType& it);

        bool                        isDouble() { return true; }

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring        getTypeStr() {return L"constant";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring        getShortTypeStr() {return L"s";}
    protected :
        RealType                    getType(void);

    private :
        virtual void                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

        virtual double              getNullValue();
        virtual Double*             createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual double              copyValue(double _dblData);
        virtual void                deleteAll();
        virtual void                deleteImg();
        virtual double*             allocData(int _iSize);


    };
}

#ifdef _MSC_VER
template class TYPES_IMPEXP types::ArrayOf<double>; //Double
#endif
#endif /* !__DOUBLE_HXX__ */
