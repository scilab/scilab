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

#ifndef DIAGRAM_HXX_
#define DIAGRAM_HXX_

#include <string>
#include <vector>

#include "utilities.hxx"
#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

struct SimulationConfig
{
    double final_time;          //!< Final simulation time.
    double absolute_tolerance;  //!< Integrator absolute tolerance for the numerical solver.
    double relative_tolerance;  //!< Integrator relative tolerance for the numerical solver.
    double time_tolerance;      //!< Tolerance on time.
    double delta_t;             //!< Maximum integration time interval.
    double realtime_scale;      //!< Real-time scaling; the value 0 corresponds to no real-time scaling.
    double solver;              //!< Current numerical solver.
    double delta_h;             //!< Maximum step interval.

    SimulationConfig() : final_time(100000), absolute_tolerance(1e-6), relative_tolerance(1e-6),
        time_tolerance(1e-10), delta_t(final_time + 1), realtime_scale(0), solver(1), delta_h(0) {};

    SimulationConfig(const SimulationConfig& p) :  final_time(p.final_time), absolute_tolerance(p.absolute_tolerance),
        relative_tolerance(p.relative_tolerance), time_tolerance(p.time_tolerance), delta_t(p.delta_t),
        realtime_scale(p.realtime_scale), solver(p.solver), delta_h(p.delta_h) {};

    SimulationConfig(const std::vector<double>& p) : final_time(p[0]), absolute_tolerance(p[1]), relative_tolerance(p[2]),
        time_tolerance(p[3]), delta_t(p[4]), realtime_scale(p[5]), solver(p[6]), delta_h(p[7]) {};

    void fill(std::vector<double>& p) const
    {
        p.resize(8);
        p[0] = final_time;
        p[1] = absolute_tolerance;
        p[2] = relative_tolerance;
        p[3] = time_tolerance;
        p[4] = delta_t;
        p[5] = realtime_scale;
        p[6] = solver;
        p[7] = delta_h;
    }
    bool operator==(const SimulationConfig& p) const
    {
        return final_time == p.final_time && absolute_tolerance == p.absolute_tolerance &&
               relative_tolerance == p.relative_tolerance && time_tolerance == p.time_tolerance &&
               delta_t == p.delta_t && realtime_scale == p.realtime_scale && solver == p.solver && delta_h == p.delta_h;
    }
};

class Diagram: public BaseObject
{
public:
    Diagram() : BaseObject(DIAGRAM), m_title("Untitled"), m_path(), m_properties(), m_debugLevel(), m_context(), m_children(), m_version()
    {
        m_color = { -1, 1};
    }

    Diagram(const Diagram& o) : BaseObject(DIAGRAM), m_title(o.m_title), m_path(o.m_path), m_color(o.m_color), m_properties(o.m_properties),
        m_debugLevel(o.m_debugLevel), m_context(o.m_context), m_children(o.m_children), m_version(o.m_version) {};

private:
    friend class ::org_scilab_modules_scicos::Model;

    void getChildren(std::vector<ScicosID>& c) const
    {
        c = m_children;
    }

    update_status_t setChildren(const std::vector<ScicosID>& c)
    {
        if (c == m_children)
        {
            return NO_CHANGES;
        }

        m_children = c;
        return SUCCESS;
    }

    void getContext(std::vector<std::string>& data) const
    {
        data = m_context;
    }

    update_status_t setContext(const std::vector<std::string>& data)
    {
        if (data == m_context)
        {
            return NO_CHANGES;
        }

        m_context = data;
        return SUCCESS;
    }

    const std::vector<Datatype*>& getDatatypes() const
    {
        return m_datatypes;
    }

    void setDatatypes(const std::vector<Datatype*>& datatypes)
    {
        this->m_datatypes = datatypes;
    }

    void getProperties(std::vector<double>& v) const
    {
        m_properties.fill(v);
    }

    update_status_t setProperties(const std::vector<double>& v)
    {
        if (v.size() != 8)
        {
            return FAIL;
        }

        SimulationConfig p = SimulationConfig(v);
        if (p == m_properties)
        {
            return NO_CHANGES;
        }

        m_properties = p;
        return SUCCESS;
    }

    void getDebugLevel(int& data) const
    {
        data = m_debugLevel;
    }

    update_status_t setDebugLevel(const int& data)
    {
        if (data == m_debugLevel)
        {
            return NO_CHANGES;
        }

        m_debugLevel = data;
        return SUCCESS;
    }

    void getTitle(std::string& data) const
    {
        data = m_title;
    }

    update_status_t setTitle(const std::string& data)
    {
        if (data == m_title)
        {
            return NO_CHANGES;
        }

        m_title = data;
        return SUCCESS;
    }

    void getPath(std::string& data) const
    {
        data = m_path;
    }

    update_status_t setPath(const std::string& data)
    {
        if (data == m_path)
        {
            return NO_CHANGES;
        }

        m_path = data;
        return SUCCESS;
    }

    void getColor(std::vector<int>& data) const
    {
        data = m_color;
    }

    update_status_t setColor(const std::vector<int>& data)
    {
        if (data == m_color)
        {
            return NO_CHANGES;
        }

        m_color = data;
        return SUCCESS;
    }

    void getVersionNumber(std::string& data) const
    {
        data = m_version;
    }

    update_status_t setVersionNumber(const std::string& data)
    {
        if (data == m_version)
        {
            return NO_CHANGES;
        }

        m_version = data;
        return SUCCESS;
    }

private:
    std::string m_title;
    std::string m_path;
    std::vector<int> m_color;
    SimulationConfig m_properties;
    int m_debugLevel;
    std::vector<std::string> m_context;

    std::vector<ScicosID> m_children;
    std::vector<Datatype*> m_datatypes;

    std::string m_version;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* DIAGRAM_HXX_ */
