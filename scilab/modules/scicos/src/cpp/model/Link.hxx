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

#ifndef LINK_HXX_
#define LINK_HXX_

#include <string>
#include <vector>

#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

/*
 * List of all possible values for a Link.
 */
enum link_kind_t
{
    activation = -1,
    regular = 1,
    implicit = 2
};

class Link: public BaseObject
{
    friend class ::org_scilab_modules_scicos::Model;

private:
    Link() : BaseObject(LINK), parentDiagram(0), sourcePort(0), destinationPort(0), controlPoints(),
        label(), thick(std::vector<double>(2)), color(1), linkKind(regular) {};
    Link(const Link& o) : BaseObject(LINK), parentDiagram(o.parentDiagram), sourcePort(o.sourcePort), destinationPort(o.destinationPort),
        controlPoints(o.controlPoints), label(o.label), thick(o.thick), color(o.color), linkKind(o.linkKind) {};
    ~Link() {}

    ScicosID getParentDiagram() const
    {
        return parentDiagram;
    }

    void setParentDiagram(ScicosID parentDiagram)
    {
        this->parentDiagram = parentDiagram;
    }

    void getControlPoints(std::vector<double>& v) const
    {
        v = controlPoints;
    }

    update_status_t setControlPoints(const std::vector<double>& v)
    {
        if (v == controlPoints)
        {
            return NO_CHANGES;
        }

        controlPoints = v;
        return SUCCESS;
    }

    void getLabel(std::string& data) const
    {
        data = label;
    }

    update_status_t setLabel(const std::string& data)
    {
        if (data == label)
        {
            return NO_CHANGES;
        }

        label = data;
        return SUCCESS;
    }

    void getThick(std::vector<double>& v) const
    {
        v = thick;
    }

    update_status_t setThick(const std::vector<double>& v)
    {
        if (v.size() != 2)
        {
            return FAIL;
        }

        if (v == thick)
        {
            return NO_CHANGES;
        }

        thick = v;
        return SUCCESS;
    }

    void getColor(int& c) const
    {
        c = color;
    }

    update_status_t setColor(const int c)
    {
        if (c == color)
        {
            return NO_CHANGES;
        }

        color = c;
        return SUCCESS;
    }

    void getKind(int& k) const
    {
        k = linkKind;
    }

    update_status_t setKind(const int k)
    {
        if (k == linkKind)
        {
            return NO_CHANGES;
        }

        // Only accepting Scilab Link kind values
        switch (k)
        {
            case activation:
                linkKind = activation;
                break;
            case regular:
                linkKind = regular;
                break;
            case implicit:
                linkKind = implicit;
                break;
            default:
                return FAIL;
        }
        return SUCCESS;
    }

    void getSourcePort(ScicosID& sp) const
    {
        sp = sourcePort;
    }

    update_status_t setSourcePort(const ScicosID sp)
    {
        if (sp == sourcePort)
        {
            return NO_CHANGES;
        }

        sourcePort = sp;
        return SUCCESS;
    }

    void getDestinationPort(ScicosID& dp) const
    {
        dp = destinationPort;
    }

    update_status_t setDestinationPort(const ScicosID dp)
    {
        if (dp == destinationPort)
        {
            return NO_CHANGES;
        }

        destinationPort = dp;
        return SUCCESS;
    }

private:
    ScicosID parentDiagram;

    ScicosID sourcePort;
    ScicosID destinationPort;

    // used to store, user-defined control points
    std::vector<double> controlPoints;

    std::string label;
    std::vector<double> thick;
    int color;
    link_kind_t linkKind;

};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* LINK_HXX_ */
