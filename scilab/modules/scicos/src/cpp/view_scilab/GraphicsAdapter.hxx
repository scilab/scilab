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

#ifndef GRAPHICSADAPTER_HXX_
#define GRAPHICSADAPTER_HXX_

#include <string>

#include "internal.hxx"

#include "BaseAdapter.hxx"
#include "model/Block.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class GraphicsAdapter : public BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>
{
public:
    GraphicsAdapter();
    GraphicsAdapter(const Controller& c, model::Block* adaptee);
    ~GraphicsAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"graphics";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    types::InternalType* getGrIContent() const;
    void setGrIContent(types::InternalType* v);

private:
    types::InternalType* gr_i_content;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* GRAPHICSADAPTER_HXX_ */
