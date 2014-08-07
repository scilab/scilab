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

#ifndef BLOCKADAPTER_HXX_
#define BLOCKADAPTER_HXX_

#include <string>

#include "utilities.hxx"
#include "BaseAdapter.hxx"
#include "model/Block.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class BlockAdapter : public BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>
{
public:
    BlockAdapter(const BlockAdapter& o);
    BlockAdapter(org_scilab_modules_scicos::model::Block* o);
    BlockAdapter(ScicosID uid, kind_t k);
    ~BlockAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"Block";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    types::InternalType* getDocContent() const;
    void setDocContent(types::InternalType* v);
private:
    types::InternalType* doc_content;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* BLOCKADAPTER_HXX_ */
