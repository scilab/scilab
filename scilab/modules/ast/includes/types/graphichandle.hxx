/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __GRAPHICHANDLE_HXX__
#define __GRAPHICHANDLE_HXX__

#include "arrayof.hxx"
#include "bool.hxx"

namespace types
{
class EXTERN_AST GraphicHandle : public ArrayOf<long long>
{
public :
    GraphicHandle(long long _handle);
    GraphicHandle(int _iRows, int _iCols);
    GraphicHandle(int _iDims, const int* _piDims);

    virtual                 ~GraphicHandle();

    GraphicHandle*          clone() override;

    void                    whoAmI() override;

    bool                    isHandle() override
    {
        return true;
    }

    bool                    isContainer() override
    {
        return true;
    }

    bool                    operator==(const InternalType& it) override;
    bool                    operator!=(const InternalType& it) override;

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring     getTypeStr() const override
    {
        return L"handle";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring     getShortTypeStr() const override
    {
        return L"h";
    }

    bool                    hasToString() override
    {
        return false;
    }

    bool isTrue() override
    {
        return false;
    }

    virtual bool neg(InternalType *& /*out*/) override
    {
        return false;
    }

    virtual bool transpose(InternalType *& out) override;

    virtual bool isFieldExtractionOverloadable() const override
    {
        return true;
    }

    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e) override ;

    bool getMemory(long long* _piSize, long long* _piSizePlusType) override;

protected :
    inline ScilabType		getType(void) override
    {
        return ScilabHandle;
    }
    inline ScilabId         getId(void) override
    {
        return isScalar() ? IdScalarHandle : IdHandle;
    }

private :
    virtual bool            subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) override;

    virtual long long       getNullValue() override;
    virtual GraphicHandle*  createEmpty(int _iDims, int* _piDims, bool _bComplex = false) override;
    virtual long long       copyValue(long long _handle) override;
    virtual void            deleteAll() override;
    virtual void            deleteImg() override;
    virtual long long*      allocData(int _iSize) override;
};
}

#ifdef _MSC_VER
template class types::ArrayOf<long long>; //GraphicHandle
#endif
#endif /* !__GRAPHICHANDLE_HXX__ */
