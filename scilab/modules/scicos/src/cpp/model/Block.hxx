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

#ifndef BLOCK_HXX_
#define BLOCK_HXX_

#include <string>
#include <vector>

#include "utilities.hxx"
#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

struct Parameter
{
    std::vector<double> rpar;
    std::vector<int> ipar;
    std::vector<double> opar;

    // opar default value is an empty list encoded by var2vec()
    Parameter() : rpar(), ipar()
    {
        opar = {15, 0};
    }
};

struct State
{
    std::vector<double> state;
    std::vector<double> dstate;
    std::vector<double> odstate;

    // odstate default value is an empty list encoded by var2vec()
    State() : state(), dstate()
    {
        odstate = {15, 0};
    }
};

/**
 * Mask list for all possible block scheduling descriptor from the simulator point of view.
 *
 * Examples:
 *  * CONST_m == 0
 *  * SUMMATION == DEP_U
 *  * CLR == DEP_T
 *  * SWITCH_f == DEP_U | DEP_T
 */
enum dep_ut_t
{
    DEP_U       = 1 << 0, //!< y=f(u)
    DEP_T       = 1 << 1, //!< y=f(x)
};

enum blocktype_t
{
    BLOCKTYPE_C = 'c', //!< N/A ; dummy value used to represent a 'c' blocktype (eg. not 'd')
    BLOCKTYPE_D = 'd', //!< N/A ; dummy value used to represent a 'd' blocktype (eg. not 'c')
    BLOCKTYPE_H = 'h', //!< N/A ; used to represent blocks composed by blocks
    BLOCKTYPE_L = 'l', //!< synchronization block ; ifthenelse and eselect
    BLOCKTYPE_M = 'm', //!< memorization block ; see the Scicos original paper
    BLOCKTYPE_X = 'x', //!< derivable block without state ; these blocks will be treated as if they contain a state.
    BLOCKTYPE_Z = 'z', //!< zero-crossing block ; see the Scicos original paper.
};

struct Descriptor
{
    std::string functionName;
    int functionApi;

    char dep_ut;            //!< dep_ut_t masked value
    char blocktype;         //!< one of blocktype_t value

    Descriptor() : functionName(), functionApi(0), dep_ut(0), blocktype(BLOCKTYPE_C) {}
};

class Block: public BaseObject
{
public:
    Block() : BaseObject(BLOCK), m_parentDiagram(ScicosID()), m_interfaceFunction(), m_geometry(),
        m_description(), m_label(), m_style(), m_uid(), m_sim(), m_in(), m_out(), m_ein(), m_eout(),
        m_parameter(), m_state(), m_parentBlock(ScicosID()), m_children(), m_childrenColor(), m_context(), m_portReference(ScicosID())
    {
        // m_exprs default value is an empty matrix encoded by var2vec()
        m_exprs = {1, 2, 0, 0, 0};
        // m_equations default value is an empty list encoded by var2vec()
        m_equations = {15, 0};
        m_nmode = {0};
        m_nzcross = {0};
        m_childrenColor = { -1, 1};
    }
    Block(const Block& o) : BaseObject(BLOCK), m_parentDiagram(o.m_parentDiagram), m_interfaceFunction(o.m_interfaceFunction), m_geometry(o.m_geometry),
        m_exprs(o.m_exprs), m_description(o.m_description), m_label(o.m_label), m_style(o.m_style), m_nzcross(o.m_nzcross), m_nmode(o.m_nmode), m_equations(o.m_equations), m_uid(o.m_uid),
        m_sim(o.m_sim), m_in(o.m_in), m_out(o.m_out), m_ein(o.m_ein), m_eout(o.m_eout), m_parameter(o.m_parameter), m_state(o.m_state), m_parentBlock(o.m_parentBlock),
        m_children(o.m_children), m_childrenColor(o.m_childrenColor), m_context(o.m_context), m_portReference(o.m_portReference) {}

private:
    friend class ::org_scilab_modules_scicos::Model;

    void getChildren(std::vector<ScicosID>& v) const
    {
        v = this->m_children;
    }

    update_status_t setChildren(const std::vector<ScicosID>& children)
    {
        if (children == this->m_children)
        {
            return NO_CHANGES;
        }

        this->m_children = children;
        return SUCCESS;
    }

    void getChildrenColor(std::vector<int>& data) const
    {
        data = this->m_childrenColor;
    }

    update_status_t setChildrenColor(const std::vector<int>& data)
    {
        if (data == this->m_childrenColor)
        {
            return NO_CHANGES;
        }

        this->m_childrenColor = data;
        return SUCCESS;
    }

    void getGeometry(std::vector<double>& v) const
    {
        m_geometry.fill(v);
    }

    update_status_t setGeometry(const std::vector<double>& v)
    {
        if (v.size() != 4)
        {
            return FAIL;
        }

        Geometry g = Geometry(v);
        if (g == m_geometry)
        {
            return NO_CHANGES;
        }

        m_geometry = g;
        return SUCCESS;
    }

    void getExprs(std::vector<double>& data) const
    {
        data = m_exprs;
    }

    update_status_t setExprs(const std::vector<double>& data)
    {
        if (data == m_exprs)
        {
            return NO_CHANGES;
        }

        m_exprs = data;
        return SUCCESS;
    }

    void getDescription(std::string& data) const
    {
        data = m_description;
    }

    update_status_t setDescription(const std::string& data)
    {
        if (data == m_description)
        {
            return NO_CHANGES;
        }

        m_description = data;
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

    void getIn(std::vector<ScicosID>& v) const
    {
        v = m_in;
    }

    update_status_t setIn(const std::vector<ScicosID>& in)
    {
        if (in == this->m_in)
        {
            return NO_CHANGES;
        }

        this->m_in = in;
        return SUCCESS;
    }

    void getInterfaceFunction(std::string& fun) const
    {
        fun = m_interfaceFunction;
    }

    update_status_t setInterfaceFunction(const std::string& fun)
    {
        if (fun == m_interfaceFunction)
        {
            return NO_CHANGES;
        }

        m_interfaceFunction = fun;
        return SUCCESS;
    }

    void getOut(std::vector<ScicosID>& v) const
    {
        v = m_out;
    }

    update_status_t setOut(const std::vector<ScicosID>& out)
    {
        if (out == this->m_out)
        {
            return NO_CHANGES;
        }

        this->m_out = out;
        return SUCCESS;
    }

    void getEin(std::vector<ScicosID>& v) const
    {
        v = m_ein;
    }

    update_status_t setEin(const std::vector<ScicosID>& ein)
    {
        if (ein == this->m_ein)
        {
            return NO_CHANGES;
        }

        this->m_ein = ein;
        return SUCCESS;
    }

    void getEout(std::vector<ScicosID>& v) const
    {
        v = m_eout;
    }

    update_status_t setEout(const std::vector<ScicosID>& eout)
    {
        if (eout == this->m_eout)
        {
            return NO_CHANGES;
        }

        this->m_eout = eout;
        return SUCCESS;
    }

    const Parameter& getParameter() const
    {
        return m_parameter;
    }

    void setParameter(const Parameter& parameter)
    {
        this->m_parameter = parameter;
    }

    void getParentBlock(ScicosID& v) const
    {
        v = m_parentBlock;
    }

    update_status_t setParentBlock(ScicosID parentBlock)
    {
        if (parentBlock == this->m_parentBlock)
        {
            return NO_CHANGES;
        }

        this->m_parentBlock = parentBlock;
        return SUCCESS;
    }

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

    void getPortReference(ScicosID& v) const
    {
        v = m_portReference;
    }

    update_status_t setPortReference(const ScicosID v)
    {
        if (v == m_portReference)
        {
            return NO_CHANGES;
        }

        m_portReference = v;
        return SUCCESS;
    }

    const Descriptor& getSim() const
    {
        return m_sim;
    }

    void setSim(const Descriptor& sim)
    {
        this->m_sim = sim;
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

    void getNZcross(std::vector<int>& data) const
    {
        data = m_nzcross;
    }

    update_status_t setNZcross(const std::vector<int>& data)
    {
        if (data == m_nzcross)
        {
            return NO_CHANGES;
        }

        m_nzcross = data;
        return SUCCESS;
    }

    void getNMode(std::vector<int>& data) const
    {
        data = m_nmode;
    }

    update_status_t setNMode(const std::vector<int>& data)
    {
        if (data == m_nmode)
        {
            return NO_CHANGES;
        }

        m_nmode = data;
        return SUCCESS;
    }

    void getEquations(std::vector<double>& data) const
    {
        data = m_equations;
    }

    update_status_t setEquations(const std::vector<double>& data)
    {
        if (data == m_equations)
        {
            return NO_CHANGES;
        }

        m_equations = data;
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

    void getRpar(std::vector<double>& data) const
    {
        data = m_parameter.rpar;
    }

    update_status_t setRpar(const std::vector<double>& data)
    {
        if (data == m_parameter.rpar)
        {
            return NO_CHANGES;
        }

        m_parameter.rpar = data;
        return SUCCESS;
    }

    void getIpar(std::vector<int>& data) const
    {
        data = m_parameter.ipar;
    }

    update_status_t setIpar(const std::vector<int>& data)
    {
        if (data == m_parameter.ipar)
        {
            return NO_CHANGES;
        }

        m_parameter.ipar = data;
        return SUCCESS;
    }

    void getOpar(std::vector<double>& data) const
    {
        data = m_parameter.opar;
    }

    update_status_t setOpar(const std::vector<double>& data)
    {
        if (data == m_parameter.opar)
        {
            return NO_CHANGES;
        }

        m_parameter.opar = data;
        return SUCCESS;
    }

    void getSimFunctionName(std::string& data) const
    {
        data = m_sim.functionName;
    }

    update_status_t setSimFunctionName(const std::string& data)
    {
        if (data == m_sim.functionName)
        {
            return NO_CHANGES;
        }

        m_sim.functionName = data;
        return SUCCESS;
    }

    void getSimFunctionApi(int& data) const
    {
        data = m_sim.functionApi;
    }

    update_status_t setSimFunctionApi(const int data)
    {
        if (data == m_sim.functionApi)
        {
            return NO_CHANGES;
        }

        m_sim.functionApi = data;
        return SUCCESS;
    }

    void getSimBlocktype(std::string& data) const
    {
        data = std::string(1, m_sim.blocktype);
    }

    update_status_t setSimBlocktype(const std::string data)
    {
        if (data.size() != 1)
        {
            return FAIL;
        }

        char c = *(data.c_str());

        if (c == m_sim.blocktype)
        {
            return NO_CHANGES;
        }

        switch (c)
        {
            case BLOCKTYPE_C:
            case BLOCKTYPE_D:
            case BLOCKTYPE_H:
            case BLOCKTYPE_L:
            case BLOCKTYPE_M:
            case BLOCKTYPE_X:
            case BLOCKTYPE_Z:
                m_sim.blocktype = c;
                return SUCCESS;
            default:
                return FAIL;
        }
    }

    void getSimDepUT(std::vector<int>& data) const
    {
        data.resize(2, 0);
        switch (m_sim.dep_ut)
        {
            case DEP_U & DEP_T:
                // data is already set to [0 0] here.
                break;
            case DEP_U:
                data[0] = 1;
                break;
            case DEP_T:
                data[1] = 1;
                break;
            case DEP_U | DEP_T:
                data[0] = 1;
                data[1] = 1;
                break;
            default:
                break;
        }
    }

    update_status_t setSimDepUT(const std::vector<int>& data)
    {
        if (data.size() != 2)
        {
            return FAIL;
        }

        int dep = DEP_U & DEP_T;
        if (data[0])
        {
            if (data[1])
            {
                dep = DEP_U | DEP_T;
            }
            else
            {
                dep = DEP_U;
            }
        }
        else if (data[1])
        {
            dep = DEP_T;
        }

        if (dep == m_sim.dep_ut)
        {
            return NO_CHANGES;
        }

        m_sim.dep_ut = dep;
        return SUCCESS;
    }

    void getState(std::vector<double>& data) const
    {
        data = m_state.state;
    }

    update_status_t setState(const std::vector<double>& data)
    {
        if (data == m_state.state)
        {
            return NO_CHANGES;
        }

        m_state.state = data;
        return SUCCESS;
    }

    void getDState(std::vector<double>& data) const
    {
        data = m_state.dstate;
    }

    update_status_t setDState(const std::vector<double>& data)
    {
        if (data == m_state.dstate)
        {
            return NO_CHANGES;
        }

        m_state.dstate = data;
        return SUCCESS;
    }

    void getODState(std::vector<double>& data) const
    {
        data = m_state.odstate;
    }

    update_status_t setODState(const std::vector<double>& data)
    {
        if (data == m_state.odstate)
        {
            return NO_CHANGES;
        }

        m_state.odstate = data;
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

private:
    ScicosID m_parentDiagram;
    std::string m_interfaceFunction;
    Geometry m_geometry;
    std::vector<double> m_exprs;
    std::string m_description;
    std::string m_label;
    std::string m_style;
    std::vector<int> m_nzcross;
    std::vector<int> m_nmode;
    std::vector<double> m_equations;
    std::string m_uid;

    Descriptor m_sim;

    std::vector<ScicosID> m_in;
    std::vector<ScicosID> m_out;
    std::vector<ScicosID> m_ein;
    std::vector<ScicosID> m_eout;

    Parameter m_parameter;
    State m_state;

    /**
     * SuperBlock: the blocks, links and so on contained into this block
     */
    ScicosID m_parentBlock;
    std::vector<ScicosID> m_children;
    std::vector<int> m_childrenColor;
    std::vector<std::string> m_context;

    /**
     * I/O Blocks: the corresponding parent port
     */
    ScicosID m_portReference;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* BLOCK_HXX_ */
