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

#ifndef DIAGRAMADAPTER_HXX_
#define DIAGRAMADAPTER_HXX_

#include <string>
#include <vector>

#include "internal.hxx"
#include "list.hxx"

#include "adapters_utilities.hxx"
#include "BaseAdapter.hxx"
#include "model/Diagram.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class DiagramAdapter : public BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>
{
public:
    DiagramAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee);
    DiagramAdapter(const DiagramAdapter& adapter);
    ~DiagramAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"diagram";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    types::List* getListObjects() const;
    void setListObjects(types::List* v);

    std::vector<link_t> getFrom() const;
    void setFrom(const std::vector<link_t>& from);
    std::vector<link_t> getTo() const;
    void setTo(const std::vector<link_t>& to);

    types::InternalType* getContribContent() const;
    void setContribContent(types::InternalType* v);

private:
    // Infos for the child links
    std::vector<link_t> from_vec;
    std::vector<link_t> to_vec;

    // Infos for the child elements
    types::List* list_objects;

    // User-defined contrib
    types::InternalType* contrib_content;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* MODELADAPTER_HXX_ */
