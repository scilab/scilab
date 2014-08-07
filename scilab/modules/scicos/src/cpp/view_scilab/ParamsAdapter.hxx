/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef PARAMSADAPTER_HXX_
#define PARAMSADAPTER_HXX_

#include <string>

#include "BaseAdapter.hxx"

#include "model/Diagram.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class ParamsAdapter : public BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>
{
public:
    ParamsAdapter(const ParamsAdapter& o);
    ParamsAdapter(org_scilab_modules_scicos::model::Diagram* o);
    ~ParamsAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"params";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* PARAMSADAPTER_HXX_ */
