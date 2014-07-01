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

#ifndef DIAGRAM_HXX_
#define DIAGRAM_HXX_

#include <vector>

#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

struct SimulationConfig
{
    double final_time;

    // FIXME: add more properties there
};

class Diagram: public BaseObject
{
    friend class ::org_scilab_modules_scicos::Model;

private:
    Diagram() : BaseObject(DIAGRAM) {};
    Diagram(const Diagram& o)  : BaseObject(DIAGRAM) {};
    ~Diagram() {}

    const std::vector<ScicosID>& getChildren() const
    {
        return children;
    }

    void setChildren(const std::vector<ScicosID>& children)
    {
        this->children = children;
    }

    const std::vector<Datatype*>& getDatatypes() const
    {
        return datatypes;
    }

    void setDatatypes(const std::vector<Datatype*>& datatypes)
    {
        this->datatypes = datatypes;
    }

    const SimulationConfig& getProperties() const
    {
        return properties;
    }

    void setProperties(const SimulationConfig& properties)
    {
        this->properties = properties;
    }

private:
    SimulationConfig properties;

    std::vector<ScicosID> children;
    std::vector<Datatype*> datatypes;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* DIAGRAM_HXX_ */
