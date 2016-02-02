/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
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
    TextAdapter(const Controller& c, org_scilab_modules_scicos::model::Annotation* adaptee);
    TextAdapter(const TextAdapter& adapter);
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
