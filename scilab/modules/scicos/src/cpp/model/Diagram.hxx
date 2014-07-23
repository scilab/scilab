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
    double final_time;          //!< Final simulation time.
    double absolute_tolerance;  //!< Integrator absolute tolerance for the numerical solver.
    double relative_tolerance;  //!< Integrator relative tolerance for the numerical solver.
    double time_tolerance;      //!< Tolerance on time.
    double delta_t;             //!< Maximum integration time interval.
    double delta_h;             //!< Maximum step interval.
    double realtime_scale;      //!< Real-time scaling; the value 0 corresponds to no real-time scaling.
    double solver;              //!< Current numerical solver.
};

class Diagram: public BaseObject
{
private:
    friend class ::org_scilab_modules_scicos::Model;

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

    const std::vector<std::string>& getContext() const
    {
        return context;
    }

    void setContext(const std::vector<std::string>& context)
    {
        this->context = context;
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

    const std::string& getTitle() const
    {
        return title;
    }

    void setTitle(const std::string& title)
    {
        this->title = title;
    }

    const std::string& getVersion() const
    {
        return version;
    }

    void setVersion(const std::string& version)
    {
        this->version = version;
    }

private:
    std::string title;
    SimulationConfig properties;
    std::vector<std::string> context;

    std::vector<ScicosID> children;
    std::vector<Datatype*> datatypes;

    std::string version;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* DIAGRAM_HXX_ */
