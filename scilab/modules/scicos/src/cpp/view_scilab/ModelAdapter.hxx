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

#ifndef MODELADAPTER_HXX_
#define MODELADAPTER_HXX_

#include <string>
#include <sstream>

#include "BaseAdapter.hxx"

#include "model/Block.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class ModelAdapter : public BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>
{
public:
    ModelAdapter(const ModelAdapter& o);
    ModelAdapter(org_scilab_modules_scicos::model::Block* o);
    ~ModelAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"model";
    }

    bool toString(std::wostringstream& ostr);
    std::wstring getTypeStr();
    std::wstring getShortTypeStr();
};

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* MODELADAPTER_HXX_ */
