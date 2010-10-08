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
                            Cell(int _iRows, int _iCols);
                            ~Cell(); 

    private :
                            Cell(Cell* _oCellCopyMe);
        void                createCell(int _iRows, int _iCols);

    public :
        int                 size_get(); 

        void                whoAmI(void) { std::cout << "types::Cell"; };

        RealType            getType(void) { return RealCell; } 

        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        Cell*               clone();

        std::wstring        toString(int _iPrecision, int _iLineLen);

        Cell*               getAsCell(void) { return this; }

        InternalType*       get(int _iRows, int _iCols);
        InternalType*       get(int _iIndex);
        bool                set(int _iRows, int _iCols, InternalType* _pIT);
        bool                set(int _iIndex, InternalType* _pIT);


        bool                resize(int _iNewRows, int _iNewCols);
        bool                append(int _iRows, int _iCols, Cell *_poSource);

        bool                operator==(const InternalType& it);
        bool                operator!=(const InternalType& it);

        bool                insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        bool                insert_cell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        static Cell*        insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        Cell*               extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);

        vector<InternalType*> extract_cell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring getTypeStr() {return L"cell";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring getShortTypeStr() {return L"ce";};

    private :
        InternalType**      m_plData;
    };
}

#endif /* !__CELL_HXX__ */
