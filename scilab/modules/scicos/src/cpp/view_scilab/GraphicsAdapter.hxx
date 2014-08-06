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

#ifndef GRAPHICSADAPTER_HXX_
#define GRAPHICSADAPTER_HXX_

#include <string>
#include <sstream>

#include "BaseAdapter.hxx"
#include "model/Block.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class GraphicsAdapter : public BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>
{
public:
    GraphicsAdapter(const GraphicsAdapter& o);
    GraphicsAdapter(org_scilab_modules_scicos::model::Block* o);
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
