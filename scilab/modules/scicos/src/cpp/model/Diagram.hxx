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

#ifndef DIAGRAM_HXX_
#define DIAGRAM_HXX_

#include <string>
#include <vector>

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
    double delta_h;             //!< Maximum step interval.
    double realtime_scale;      //!< Real-time scaling; the value 0 corresponds to no real-time scaling.
    double solver;              //!< Current numerical solver.

    SimulationConfig() : final_time(100000), absolute_tolerance(1e-6), relative_tolerance(1e-6),
        time_tolerance(1e-10), delta_t(final_time + 1), delta_h(0), realtime_scale(1), solver(0) {};

    SimulationConfig(const SimulationConfig& p) :  final_time(p.final_time), absolute_tolerance(p.absolute_tolerance),
        relative_tolerance(p.relative_tolerance), time_tolerance(p.time_tolerance), delta_t(p.delta_t),
        delta_h(p.delta_h), realtime_scale(p.realtime_scale), solver(p.solver) {};

    SimulationConfig(const std::vector<double>& p) : final_time(p[0]), absolute_tolerance(p[1]), relative_tolerance(p[2]),
        time_tolerance(p[3]), delta_t(p[4]), delta_h(p[5]), realtime_scale(p[6]), solver(p[7]) {};

    void fill(std::vector<double>& p) const
    {
        p.resize(8);
        p[0] = final_time;
        p[1] = absolute_tolerance;
        p[2] = relative_tolerance;
        p[3] = time_tolerance;
        p[4] = delta_t;
        p[5] = delta_h;
        p[6] = realtime_scale;
        p[7] = solver;
    }
    bool operator==(const SimulationConfig& p) const
    {
        return final_time == p.final_time && absolute_tolerance == p.absolute_tolerance &&
               relative_tolerance == p.relative_tolerance && time_tolerance == p.time_tolerance &&
               delta_t == p.delta_t && delta_h == p.delta_h && realtime_scale == p.realtime_scale && solver == p.solver;
    }
};

class Diagram: public BaseObject
{
private:
    friend class ::org_scilab_modules_scicos::Model;

    Diagram() : BaseObject(DIAGRAM), title("Untitled"), path(), properties(), context(), children(), version() {};
    Diagram(const Diagram& o) : BaseObject(DIAGRAM), title(o.title), path(o.path), properties(o.properties),
        context(o.context), children(o.children), version(o.version) {};
    ~Diagram() {}

    void getChildren(std::vector<ScicosID>& c) const
    {
        c = children;
    }

    update_status_t setChildren(const std::vector<ScicosID>& c)
    {
        if (c == children)
        {
            return NO_CHANGES;
        }

        children = c;
        return SUCCESS;
    }

    void getContext(std::vector<std::string>& data) const
    {
        data = context;
    }

    update_status_t setContext(const std::vector<std::string>& data)
    {
        if (data == context)
        {
            return NO_CHANGES;
        }

        context = data;
        return SUCCESS;
    }

    const std::vector<Datatype*>& getDatatypes() const
    {
        return datatypes;
    }

    void setDatatypes(const std::vector<Datatype*>& datatypes)
    {
        this->datatypes = datatypes;
    }

    void getProperties(std::vector<double>& v) const
    {
        properties.fill(v);
    }

    update_status_t setProperties(const std::vector<double>& v)
    {
        if (v.size() != 8)
        {
            return FAIL;
        }

        SimulationConfig p = SimulationConfig(v);
        if (p == properties)
        {
            return NO_CHANGES;
        }

        properties = p;
        return SUCCESS;
    }

    void getTitle(std::string& data) const
    {
        data = title;
    }

    update_status_t setTitle(const std::string& data)
    {
        if (data == title)
        {
            return NO_CHANGES;
        }

        title = data;
        return SUCCESS;
    }

    void getPath(std::string& data) const
    {
        data = path;
    }

    update_status_t setPath(const std::string& data)
    {
        if (data == path)
        {
            return NO_CHANGES;
        }

        path = data;
        return SUCCESS;
    }

    void getVersionNumber(std::string& data) const
    {
        data = version;
    }

    update_status_t setVersionNumber(const std::string& data)
    {
        if (data == version)
        {
            return NO_CHANGES;
        }

        version = data;
        return SUCCESS;
    }

private:
    std::string title;
    std::string path;
    SimulationConfig properties;
    std::vector<std::string> context;

    std::vector<ScicosID> children;
    std::vector<Datatype*> datatypes;

    std::string version;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* DIAGRAM_HXX_ */
