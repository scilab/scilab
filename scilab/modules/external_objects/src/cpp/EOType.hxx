/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "user.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "ScilabObjects.hxx"

namespace org_modules_external_objects
{
class EOType : public types::UserType
{
public :
    EOType(int envId, bool isNew) : id(envId), isNew(isNew)
    {
        if (pFunc == nullptr)
        {
            symbol::Context* ctx = symbol::Context::getInstance();
            types::InternalType* pIT = ctx->get(symbol::Symbol(ScilabObjects::_INVOKE_));
            if (pIT && pIT->isCallable())
            {
                pFunc = pIT->getAs<types::Callable>();
            }
        }
    }
    virtual ~EOType() {}

    inline std::wstring getTypeStr()
    {
        return L"eotype";
    }

    inline std::wstring getShortTypeStr()
    {
        return L"eo";
    }

    EOType* clone()
    {
        return new EOType(id, isNew);
    }

    inline bool hasToString()
    {
        return true;
    }

    inline bool toString(std::wostringstream& ostr)
    {
        ostr << "eotype" << std::endl;
        return true;
    }

    inline bool isInvokable() const
    {
        return true;
    }

    inline bool isAssignable()
    {
        return false;
    }

    inline bool isCallable()
    {
        return true;
    }

    bool invoke(types::typed_list & in, types::optional_list & /*opt*/, int /*_iRetCount*/, types::typed_list & out, const ast::Exp & /*e*/) override;

private:
    static types::Callable* pFunc;
    int id;
    bool isNew;

};
}
