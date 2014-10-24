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

#ifndef DIAGRAMADAPTER_HXX_
#define DIAGRAMADAPTER_HXX_

#include <string>

#include "utilities.hxx"
#include "BaseAdapter.hxx"
#include "model/Diagram.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class DiagramAdapter : public BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>
{
public:
    DiagramAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Diagram> adaptee);
    DiagramAdapter(const DiagramAdapter& adapter);
    ~DiagramAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"diagram";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    types::InternalType* getContribContent() const;
    void setContribContent(types::InternalType* v);

    std::vector<double> getFrom(int link_number) const;
    int getFromSize() const;
    void setFrom(const std::vector<double>& from_content);
    void clearFrom();
    std::vector<double> getTo(int link_number) const;
    void setTo(const std::vector<double>& to_content);
    void clearTo();

private:
    types::InternalType* contrib_content;
    std::vector< std::vector<double> > from_vec;
    std::vector< std::vector<double> > to_vec;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* MODELADAPTER_HXX_ */
