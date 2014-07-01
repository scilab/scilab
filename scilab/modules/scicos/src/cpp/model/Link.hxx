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

#ifndef LINK_HXX_
#define LINK_HXX_

#include <string>
#include <vector>
#include <utility>

#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

class Link: public BaseObject
{
    friend class ::org_scilab_modules_scicos::Model;

private:
    Link() : BaseObject(LINK), parentDiagram(0), sourcePort(0), destinationPort(0), controlPoints() {};
    Link(const Link& o) : BaseObject(LINK), parentDiagram(o.parentDiagram), sourcePort(o.sourcePort), destinationPort(o.destinationPort), controlPoints(o.controlPoints) {};
    ~Link() {}

    ScicosID getParentDiagram() const
    {
        return parentDiagram;
    }

    void setParentDiagram(ScicosID parentDiagram)
    {
        this->parentDiagram = parentDiagram;
    }

    const std::vector<std::pair<double, double> >& getControlPoints() const
    {
        return controlPoints;
    }

    void setControlPoints(const std::vector<std::pair<double, double> >& controlPoints)
    {
        this->controlPoints = controlPoints;
    }

    ScicosID getDestinationPort() const
    {
        return destinationPort;
    }

    void setDestinationPort(ScicosID destinationPort)
    {
        this->destinationPort = destinationPort;
    }

    ScicosID getSourcePort() const
    {
        return sourcePort;
    }

    void setSourcePort(ScicosID sourcePort)
    {
        this->sourcePort = sourcePort;
    }

private:
    ScicosID parentDiagram;

    ScicosID sourcePort;
    ScicosID destinationPort;

    // used to store, user-defined control points
    std::vector<std::pair<double, double> > controlPoints;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* LINK_HXX_ */
