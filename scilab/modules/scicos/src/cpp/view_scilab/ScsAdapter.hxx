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

#ifndef SCSADAPTER_HXX_
#define SCSADAPTER_HXX_

#include <string>

#include "BaseAdapter.hxx"
#include "model/Diagram.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class ScsAdapter : public BaseAdapter<ScsAdapter, org_scilab_modules_scicos::model::Diagram>
{
public:
    ScsAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee);
    ScsAdapter(const ScsAdapter& adapter);
    ~ScsAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"scs";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* SCSADAPTER_HXX_ */
