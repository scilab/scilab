/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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


//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __CELL_HXX__
#define __CELL_HXX__

#include <vector>

#include "list.hxx"
#include "arrayof.hxx"

namespace types
{
class EXTERN_AST Cell : public ArrayOf<InternalType*>
{
public :
    ~Cell();
    Cell();
    // _bInit: true, fill the struct by empty SingleStructs.
    // _bInit: false, Only alocate the array and fill each element by NULL.
    //           that mean you have to fill it by InternalTypes and increase there ref.
    Cell(int _iRows, int _iCols, InternalType** data = nullptr, bool _bInit = true);
    Cell(int _iDims, const int* _piDims, InternalType** data = nullptr, bool _bInit = true);

private :
    Cell(Cell* _oCellCopyMe);
    void createCell(int _iDims, const int* _piDims, InternalType** data, bool _bInit);
public :

    void                whoAmI(void) override
    {
        std::cout << "types::Cell";
    };

    ScilabType            getType(void) override
    {
        return ScilabCell;
    }
    ScilabId            getId(void) override
    {
        return IdCell;
    }
    bool                isCell() override
    {
        return true;
    }
    bool                isEmpty();


    /**
    ** Clone
    ** Create a new List and Copy all values.
    */
    Cell*               clone() override;

    Cell*               set(int _iRows, int _iCols, InternalType* _pIT) override;
    Cell*               set(int _iRows, int _iCols, const InternalType* _pIT);
    Cell*               set(int _iIndex, InternalType* _pIT) override;
    Cell*               set(int _iIndex, const InternalType* _pIT);
    Cell*               set(InternalType** _pIT) override;

    bool                operator==(const InternalType& it) override;
    bool                operator!=(const InternalType& it) override;

    Cell*               insertCell(typed_list* _pArgs, InternalType* _pSource);
    static Cell*        insertNewCell(typed_list* _pArgs, InternalType* _pSource);
    List*               extractCell(typed_list* _pArgs);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring getTypeStr() const override
    {
        return L"cell";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring getShortTypeStr() const override
    {
        return L"ce";
    };
    virtual bool        isContainer(void) override
    {
        return true;
    }
    bool                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) override;

    bool isTrue() override
    {
        return false;
    }

    virtual bool neg(InternalType *& /*out*/) override
    {
        return false;
    }

    virtual bool transpose(InternalType *& out) override;

    bool getMemory(long long* _piSize, long long* _piSizePlusType) override;

private :
    virtual InternalType*   getNullValue() override;
    virtual Cell*           createEmpty(int _iDims, int* _piDims, bool _bComplex = false) override;
    virtual Cell*           createEmpty() override;
    virtual InternalType*   copyValue(InternalType* _pData) override;
    virtual void            deleteAll() override;
    virtual void            deleteImg() override;
    virtual InternalType**  allocData(int _iSize) override;
    virtual void            deleteData(InternalType* _pData) override;
};
}

#ifdef _MSC_VER
template class types::ArrayOf<types::InternalType*>; //Cell
#endif
#endif /* !__CELL_HXX__ */
