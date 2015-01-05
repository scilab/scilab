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

#ifndef LINKADAPTER_HXX_
#define LINKADAPTER_HXX_

#include <string>
#include <memory>

#include "utilities.hxx"
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
    LinkAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Link> adaptee);
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
