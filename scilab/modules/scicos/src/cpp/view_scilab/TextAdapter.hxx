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

#ifndef TEXTADAPTER_HXX_
#define TEXTADAPTER_HXX_

#include <string>

#include "BaseAdapter.hxx"

#include "model/Annotation.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class TextAdapter : public BaseAdapter<TextAdapter, org_scilab_modules_scicos::model::Annotation>
{
public:
    TextAdapter(const TextAdapter& o);
    TextAdapter(org_scilab_modules_scicos::model::Annotation* o);
    ~TextAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"Text";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* MODELADAPTER_HXX_ */
