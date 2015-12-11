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
#include <vector>

#include "internal.hxx"
#include "list.hxx"

#include "BaseAdapter.hxx"
#include "adapters_utilities.hxx"
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

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    types::InternalType* getDocContent() const;
    void setDocContent(types::InternalType* v);

    types::List* getListObjects() const;
    void setListObjects(types::List* blocks);

    std::vector<link_t> getFrom() const;
    void setFrom(const std::vector<link_t>& from);
    std::vector<link_t> getTo() const;
    void setTo(const std::vector<link_t>& to);

    types::InternalType* getContribContent() const;
    void setContribContent(types::InternalType* contrib);

private:
    types::InternalType* doc_content;

    // The following content represents the child diagram (in 'model.rpar')
    // Child elements info
    types::List* list_objects;

    // Child links info
    std::vector<link_t> from_vec;
    std::vector<link_t> to_vec;
    // child diagram info
    types::InternalType* contrib_content;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* BLOCKADAPTER_HXX_ */
