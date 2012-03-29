/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/


//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __CELL_HXX__
#define __CELL_HXX__

#include <vector>

#include "list.hxx"
#include "arrayof.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP Cell : public ArrayOf<InternalType*>
    {
    public :
                            ~Cell();
                            Cell();
                            Cell(int _iRows, int _iCols);
                        	Cell(int _iDims, int* _piDims);

    private :
                            Cell(Cell* _oCellCopyMe);

    public :

        void                whoAmI(void) { std::cout << "types::Cell"; };

        RealType            getType(void) { return RealCell; }
        bool                isCell() { return true; }
        bool                isEmpty();


        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        InternalType*       clone();

        bool                set(int _iRows, int _iCols, InternalType* _pIT);
        bool                set(int _iRows, int _iCols, const InternalType* _pIT);
        bool                set(int _iIndex, InternalType* _pIT);
        bool                set(int _iIndex, const InternalType* _pIT);
        bool                set(InternalType** _pIT);

        bool                append(int _iRows, int _iCols, Cell *_poSource);

        bool                operator==(const InternalType& it);
        bool                operator!=(const InternalType& it);

        Cell*               insertCell(typed_list* _pArgs, InternalType* _pSource);
        static Cell*        insertNewCell(typed_list* _pArgs, InternalType* _pSource);
        List*               extractCell(typed_list* _pArgs);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring getTypeStr() {return L"cell";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring getShortTypeStr() {return L"ce";};
        virtual bool        isContainer(void) { return true; }
        void                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

    private :
        virtual InternalType*   getNullValue();
        virtual Cell*           createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual InternalType*   copyValue(InternalType* _pData);
        virtual void            deleteAll();
        virtual void            deleteImg();
        virtual InternalType**  allocData(int _iSize);

    };
}

#ifdef _MSC_VER
template class TYPES_IMPEXP types::ArrayOf<types::InternalType*>; //Cell
#endif
#endif /* !__CELL_HXX__ */
