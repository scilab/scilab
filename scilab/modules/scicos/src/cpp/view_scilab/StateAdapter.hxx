/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef STATEADAPTER_HXX_
#define STATEADAPTER_HXX_

#include <string>

#include "BaseAdapter.hxx"

#include "model/Diagram.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class StateAdapter : public BaseAdapter<StateAdapter, org_scilab_modules_scicos::model::Diagram>
{
public:
    StateAdapter(const StateAdapter& o);
    StateAdapter(org_scilab_modules_scicos::model::Diagram* o);
    ~StateAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"xcs";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* STATEADAPTER_HXX_ */
