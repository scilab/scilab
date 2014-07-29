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

#ifndef BLOCK_HXX_
#define BLOCK_HXX_

#include <cassert>
#include <string>
#include <vector>
#include <bitset>
#include <utility>

#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

/**
 * Scilab data that can be passed to the simulator and simulation functions.
 *
 * This used the raw scicos-sim encoding to avoid any conversion out of the model.
 */
struct list_t
{
    // re-use the scicos sim encoding
    int n;
    int* sz;
    int* typ;
    void** data;
};

struct Parameter
{
    std::vector<double> rpar;
    std::vector<int> ipar;
    list_t opar;
};

struct State
{
    std::vector<double> state;
    std::vector<int> dstate;
    list_t odstate;
};

/**
 * Mask list for all possible block scheduling descriptor from the simulator point of view.
 *
 * Examples:
 *  * CONST_m == 0
 *  * SUMMATION == DEP_U
 *  * CLR == DEP_T
 *  * SWITCH_f == DEP_U & DEP_T
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
};

/*
 * Flip and theta
 */
struct Angle
{
    bool flip;
    double theta;

    Angle() : flip(0), theta(0) {};
    Angle(const Angle& a) : flip(a.flip), theta(a.theta) {};
    Angle(const std::vector<double>& a) : flip((a[0] == 0) ? false : true), theta(a[1]) {};

    void fill(std::vector<double>& a) const
    {
        a.resize(2);
        a[0] = (flip == false) ? 0 : 1;
        a[1] = theta;
    }
    bool operator==(const Angle& a) const
    {
        return flip == a.flip && theta == a.theta;
    }
};

class Block: public BaseObject
{
private:
    friend class ::org_scilab_modules_scicos::Model;

private:
    Block() : BaseObject(BLOCK), parentDiagram(0), interfaceFunction(), geometry(),
        angle(), exprs(), label(), style(), sim(), in(), out(), ein(), eout(),
        parameter(), state(), parentBlock(0), children(), portReference(0) {};
    Block(const Block& o) : BaseObject(BLOCK), parentDiagram(o.parentDiagram), interfaceFunction(o.interfaceFunction), geometry(o.geometry),
        angle(o.angle), exprs(o.exprs), label(o.label), style(o.style), sim(o.sim), in(o.in), out(o.out), ein(o.ein), eout(o.eout),
        parameter(o.parameter), state(o.state), parentBlock(o.parentBlock), children(o.children), portReference(o.portReference) {};
    ~Block() {}

    const std::vector<ScicosID>& getChildren() const
    {
        return children;
    }

    void setChildren(const std::vector<ScicosID>& children)
    {
        this->children = children;
    }

    void getGeometry(std::vector<double>& v) const
    {
        geometry.fill(v);
    }

    update_status_t setGeometry(const std::vector<double>& v)
    {
        if (v.size() != 4)
        {
            return FAIL;
        }

        Geometry g = Geometry(v);
        if (g == geometry)
        {
            return NO_CHANGES;
        }

        geometry = g;
        return SUCCESS;
    }

    void getAngle(std::vector<double>& data) const
    {
        angle.fill(data);
    }

    update_status_t setAngle(const std::vector<double>& data)
    {
        if (data.size() != 2)
        {
            return FAIL;
        }

        Angle a = Angle(data);
        if (a == angle)
        {
            return NO_CHANGES;
        }

        angle = a;
        return SUCCESS;
    }

    void getExprs(std::vector<std::string>& data) const
    {
        data = exprs;
    }

    update_status_t setExprs(const std::vector<std::string>& data)
    {
        if (data == exprs)
        {
            return NO_CHANGES;
        }

        exprs = data;
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

    void getIn(std::vector<ScicosID>& v) const
    {
        v = in;
    }

    update_status_t setIn(const std::vector<ScicosID>& in)
    {
        if (in == this->in)
        {
            return NO_CHANGES;
        }
        this->in = in;
        return SUCCESS;
    }

    const std::string& getInterfaceFunction() const
    {
        return interfaceFunction;
    }

    void setInterfaceFunction(const std::string& interfaceFunction)
    {
        this->interfaceFunction = interfaceFunction;
    }

    void getOut(std::vector<ScicosID>& v) const
    {
        v = out;
    }

    update_status_t setOut(const std::vector<ScicosID>& out)
    {
        if (out == this->out)
        {
            return NO_CHANGES;
        }
        this->out = out;
        return SUCCESS;
    }

    void getEin(std::vector<ScicosID>& v) const
    {
        v = ein;
    }

    update_status_t setEin(const std::vector<ScicosID>& ein)
    {
        if (ein == this->ein)
        {
            return NO_CHANGES;
        }
        this->ein = ein;
        return SUCCESS;
    }

    void getEout(std::vector<ScicosID>& v) const
    {
        v = eout;
    }

    update_status_t setEout(const std::vector<ScicosID>& eout)
    {
        if (eout == this->eout)
        {
            return NO_CHANGES;
        }
        this->eout = eout;
        return SUCCESS;
    }

    const Parameter& getParameter() const
    {
        return parameter;
    }

    void setParameter(const Parameter& parameter)
    {
        this->parameter = parameter;
    }

    ScicosID getParentBlock() const
    {
        return parentBlock;
    }

    void setParentBlock(ScicosID parentBlock)
    {
        this->parentBlock = parentBlock;
    }

    ScicosID getParentDiagram() const
    {
        return parentDiagram;
    }

    void setParentDiagram(ScicosID parentDiagram)
    {
        this->parentDiagram = parentDiagram;
    }

    ScicosID getPortReference() const
    {
        return portReference;
    }

    void setPortReference(ScicosID portReference)
    {
        this->portReference = portReference;
    }

    const Descriptor& getSim() const
    {
        return sim;
    }

    void setSim(const Descriptor& sim)
    {
        this->sim = sim;
    }

    void getStyle(std::string& data) const
    {
        data = style;
    }

    update_status_t setStyle(const std::string& data)
    {
        if (data == style)
        {
            return NO_CHANGES;
        }

        style = data;
        return SUCCESS;
    }

    void getSimFunctionName(std::string& data) const
    {
        data = sim.functionName;
    }

    update_status_t setSimFunctionName(const std::string& data)
    {
        if (data == sim.functionName)
        {
            return NO_CHANGES;
        }

        sim.functionName = data;
        return SUCCESS;
    }

    void getSimFunctionApi(int& data) const
    {
        data = sim.functionApi;
    }

    update_status_t setSimFunctionApi(const int data)
    {
        if (data == sim.functionApi)
        {
            return NO_CHANGES;
        }

        sim.functionApi = data;
        return SUCCESS;
    }

private:
    ScicosID parentDiagram;
    std::string interfaceFunction;
    Geometry geometry;
    Angle angle;
    std::vector<std::string> exprs;
    std::string label;
    std::string style;

    Descriptor sim;

    std::vector<ScicosID> in;
    std::vector<ScicosID> out;
    std::vector<ScicosID> ein;
    std::vector<ScicosID> eout;

    Parameter parameter;
    State state;

    /**
     * SuperBlock: the blocks, links and so on contained into this block
     */
    ScicosID parentBlock;
    std::vector<ScicosID> children;

    /**
     * I/O Blocks: the corresponding parent port
     */
    ScicosID portReference;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* BLOCK_HXX_ */
