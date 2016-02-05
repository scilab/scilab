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

#ifndef MODELADAPTER_HXX_
#define MODELADAPTER_HXX_

#include <string>

#include "BaseAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "model/Block.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class ModelAdapter : public BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>
{
public:
    ModelAdapter();
    ModelAdapter(const Controller& c, model::Block* adaptee, DiagramAdapter* diagramAdapter);
    ~ModelAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"model";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    DiagramAdapter* getDiagram() const;
    void setDiagram(DiagramAdapter* diagramAdapter);

private:
    DiagramAdapter* m_diagramAdapter;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* MODELADAPTER_HXX_ */
