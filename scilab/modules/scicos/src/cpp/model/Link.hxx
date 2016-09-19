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

#ifndef LINK_HXX_
#define LINK_HXX_

#include <string>
#include <vector>

#include "utilities.hxx"
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
public:
    Link() : BaseObject(LINK), m_parentDiagram(ScicosID()), m_parentBlock(ScicosID()), m_sourcePort(ScicosID()), m_destinationPort(ScicosID()), m_uid(), m_controlPoints(),
        m_label(), m_style(), m_thick(2), m_color(1), m_linkKind(regular) {};
    Link(const Link& o) : BaseObject(LINK), m_parentDiagram(o.m_parentDiagram), m_parentBlock(o.m_parentBlock), m_sourcePort(o.m_sourcePort), m_destinationPort(o.m_destinationPort),  m_uid(o.m_uid),
        m_controlPoints(o.m_controlPoints), m_label(o.m_label), m_style(o.m_style), m_thick(o.m_thick), m_color(o.m_color), m_linkKind(o.m_linkKind) {};

private:
    friend class ::org_scilab_modules_scicos::Model;

    void getParentDiagram(ScicosID& v) const
    {
        v = m_parentDiagram;
    }

    update_status_t setParentDiagram(const ScicosID v)
    {
        if (v == m_parentDiagram)
        {
            return NO_CHANGES;
        }

        m_parentDiagram = v;
        return SUCCESS;
    }

    void getParentBlock(ScicosID& v) const
    {
        v = m_parentBlock;
    }

    update_status_t setParentBlock(const ScicosID v)
    {
        if (v == m_parentBlock)
        {
            return NO_CHANGES;
        }

        m_parentBlock = v;
        return SUCCESS;
    }

    void getControlPoints(std::vector<double>& v) const
    {
        v = m_controlPoints;
    }

    update_status_t setControlPoints(const std::vector<double>& v)
    {
        if (v == m_controlPoints)
        {
            return NO_CHANGES;
        }

        m_controlPoints = v;
        return SUCCESS;
    }

    void getLabel(std::string& data) const
    {
        data = m_label;
    }

    update_status_t setLabel(const std::string& data)
    {
        if (data == m_label)
        {
            return NO_CHANGES;
        }

        m_label = data;
        return SUCCESS;
    }

    void getStyle(std::string& data) const
    {
        data = m_style;
    }

    update_status_t setStyle(const std::string& data)
    {
        if (data == m_style)
        {
            return NO_CHANGES;
        }

        m_style = data;
        return SUCCESS;
    }

    void getThick(std::vector<double>& v) const
    {
        v = m_thick;
    }

    update_status_t setThick(const std::vector<double>& v)
    {
        if (v.size() != 2)
        {
            return FAIL;
        }

        if (v == m_thick)
        {
            return NO_CHANGES;
        }

        m_thick = v;
        return SUCCESS;
    }

    void getColor(int& c) const
    {
        c = m_color;
    }

    update_status_t setColor(const int c)
    {
        if (c == m_color)
        {
            return NO_CHANGES;
        }

        m_color = c;
        return SUCCESS;
    }

    void getKind(int& k) const
    {
        k = m_linkKind;
    }

    update_status_t setKind(const int k)
    {
        if (k == m_linkKind)
        {
            return NO_CHANGES;
        }

        // Only accepting Scilab Link kind values
        switch (k)
        {
            case activation:
                m_linkKind = activation;
                break;
            case regular:
                m_linkKind = regular;
                break;
            case implicit:
                m_linkKind = implicit;
                break;
            default:
                return FAIL;
        }
        return SUCCESS;
    }

    void getSourcePort(ScicosID& sp) const
    {
        sp = m_sourcePort;
    }

    update_status_t setSourcePort(const ScicosID sp)
    {
        if (sp == m_sourcePort)
        {
            return NO_CHANGES;
        }

        m_sourcePort = sp;
        return SUCCESS;
    }

    void getDestinationPort(ScicosID& dp) const
    {
        dp = m_destinationPort;
    }

    update_status_t setDestinationPort(const ScicosID dp)
    {
        if (dp == m_destinationPort)
        {
            return NO_CHANGES;
        }

        m_destinationPort = dp;
        return SUCCESS;
    }

    void getUID(std::string& data) const
    {
        data = m_uid;
    }

    update_status_t setUID(const std::string& data)
    {
        if (data == m_uid)
        {
            return NO_CHANGES;
        }

        m_uid = data;
        return SUCCESS;
    }

private:
    ScicosID m_parentDiagram;
    ScicosID m_parentBlock;

    ScicosID m_sourcePort;
    ScicosID m_destinationPort;

    std::string m_uid;
    // used to store, user-defined control points
    std::vector<double> m_controlPoints;

    std::string m_label;
    std::string m_style;
    std::vector<double> m_thick;
    int m_color;
    link_kind_t m_linkKind;

};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* LINK_HXX_ */
