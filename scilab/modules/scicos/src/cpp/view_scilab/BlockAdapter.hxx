/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
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

#ifndef BLOCKADAPTER_HXX_
#define BLOCKADAPTER_HXX_

#include <string>

#include "internal.hxx"

#include "BaseAdapter.hxx"
#include "model/Block.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class BlockAdapter : public BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>
{
public:
    BlockAdapter(const Controller& c, org_scilab_modules_scicos::model::Block* adaptee);
    BlockAdapter(const BlockAdapter& adapter);
    ~BlockAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"Block";
    }

    std::wstring getTypeStr() const override;
    std::wstring getShortTypeStr() const override;

    types::InternalType* getDocContent() const;
    void setDocContent(types::InternalType* v);

private:
    types::InternalType* doc_content;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* BLOCKADAPTER_HXX_ */
