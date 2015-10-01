/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution. The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

    InternalType* clone()
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
