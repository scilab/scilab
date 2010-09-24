/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/


#ifndef __CELL_HXX__
#define __CELL_HXX__

#include "container.hxx"
#include "symbol.hxx"

namespace types
{
    class Cell : public Container
    {
    public :
                            Cell();
                            Cell(size_t _iRows, size_t _iCols);
                            ~Cell();

    private :
                            Cell(Cell* _oCellCopyMe);
        void                createCell(size_t _iRows, size_t _iCols);

    public :
        size_t                 size_get();

        void                whoAmI(void) { std::cout << "types::Cell"; };

        RealType            getType(void) { return RealCell; }

        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        Cell*               clone();

        std::wstring        toString(int _iPrecision, int _iLineLen);

        Cell*               getAsCell(void) { return this; }

        InternalType*       get(size_t _iRows, size_t _iCols);
        InternalType*       get(size_t _iIndex);
        bool                set(size_t _iRows, size_t _iCols, InternalType* _pIT);
        bool                set(size_t _iIndex, InternalType* _pIT);


        bool                resize(size_t _iNewRows, size_t _iNewCols);
        bool                append(size_t _iRows, size_t _iCols, Cell *_poSource);

        bool                operator==(const InternalType& it);
        bool                operator!=(const InternalType& it);

        bool                insert(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        bool                insert_cell(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        static Cell*        insert_new(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        Cell*               extract(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, size_t* _piDimSize, bool _bAsVector);

        vector<InternalType*> extract_cell(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, size_t* _piDimSize, bool _bAsVector);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring getTypeStr() {return L"cell";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring getShortTypeStr() {return L"ce";};

    private :
        InternalType**      m_plData;
    };
}

#endif /* !__CELL_HXX__ */
