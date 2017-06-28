/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2017 - ESI - Antoine ELIAS
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

#ifndef __ICONVERT_CONVERTER_HXX__
#define __ICONVERT_CONVERTER_HXX__

#include "cell.hxx"
#include "context.hxx"

extern "C"
{
    #include "sciprint.h"
}
/*
castFunction(1) = {int8};
castFunction(2) = {int16};
castFunction(4) = {int32};
castFunction(8) = {int64};
castFunction(11) = {uint8};
castFunction(12) = {uint16};
castFunction(14) = {uint32};
castFunction(18) = {uint64};
castFunction(19) = {double};
*/

class iConverter
{
private:
    iConverter() 
    {
        symbol::Context* ctx = symbol::Context::getInstance();
        converter = new types::Cell(19, 1);
        converter->set(0, ctx->get(symbol::Symbol(L"int8")));
        converter->set(1, ctx->get(symbol::Symbol(L"int16")));
        converter->set(3, ctx->get(symbol::Symbol(L"int32")));
        converter->set(7, ctx->get(symbol::Symbol(L"int64")));
        converter->set(10, ctx->get(symbol::Symbol(L"uint8")));
        converter->set(11, ctx->get(symbol::Symbol(L"uint16")));
        converter->set(13, ctx->get(symbol::Symbol(L"uint32")));
        converter->set(17, ctx->get(symbol::Symbol(L"uint64")));
        converter->set(18, ctx->get(symbol::Symbol(L"double")));
        //protect it
        converter->IncreaseRef();
    }

    static iConverter* me;
    static types::Cell* converter;
public:
    virtual ~iConverter()
    {
        if (converter)
        {
            //unprotect it
            converter->DecreaseRef();
            converter->killMe();
            converter = nullptr;
        }
    }

    static types::Cell* getConverter() {
        if (me == nullptr)
        {
            me = new iConverter();
        }

        return me->converter;
    }

    static void deleteConverter()
    {
        if (me)
        {
            delete me;
            me = nullptr;
        }
    }
};

#endif /* !__ICONVERT_CONVERTER_HXX__ */
