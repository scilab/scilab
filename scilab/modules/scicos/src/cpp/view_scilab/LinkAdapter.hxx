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

#ifndef LINKADAPTER_HXX_
#define LINKADAPTER_HXX_

#include <string>

#include "adapters_utilities.hxx"
#include "BaseAdapter.hxx"
#include "model/Link.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class LinkAdapter : public BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>
{
public:
    LinkAdapter(const Controller& c, org_scilab_modules_scicos::model::Link* adaptee);
    LinkAdapter(const LinkAdapter& adapter);
    ~LinkAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"Link";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    link_t getFrom() const;
    void setFrom(const link_t& v);
    void setFromInModel(const link_t& v, Controller& controller);
    link_t getTo() const;
    void setTo(const link_t& v);
    void setToInModel(const link_t& v, Controller& controller);

private:
    link_t m_from;
    link_t m_to;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* MODELADAPTER_HXX_ */
