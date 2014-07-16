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
    BLOCKTYPE_X = 'x', //!< derivable block without state ; these blocks will be treated as if they contains a state.
    BLOCKTYPE_Z = 'z', //!< zero-crossing block ; see the Scicos original paper.
};

struct Descriptor
{
    std::string functionName;
    char functionApi;

    char dep_ut;            //!< dep_ut_t masked value
    char blocktype;         //!< one of blocktype_t value
};

class Block: public BaseObject
{
private:
    friend class ::org_scilab_modules_scicos::Model;

private:
    Block() : BaseObject(BLOCK), parentDiagram(0), interfaceFunction(), geometry(), parentBlock(0), portReference(0) {};
    Block(const Block& o) : BaseObject(BLOCK), parentDiagram(o.parentDiagram), interfaceFunction(o.interfaceFunction), geometry(o.geometry), parentBlock(o.parentBlock), portReference(o.portReference) {};
    ~Block() {}

    const std::vector<ScicosID>& getChildren() const
    {
        return children;
    }

    void setChildren(const std::vector<ScicosID>& children)
    {
        this->children = children;
    }

    const std::vector<ScicosID>& getEin() const
    {
        return ein;
    }

    void setEin(const std::vector<ScicosID>& ein)
    {
        this->ein = ein;
    }

    const std::vector<ScicosID>& getEout() const
    {
        return eout;
    }

    void setEout(const std::vector<ScicosID>& eout)
    {
        this->eout = eout;
    }

    void getGeometry(size_t* len, double** data) const
    {
        *len = 4;
        *data = geometry.copy();
    }

    update_status_t setGeometry(size_t len, double* data)
    {
        if (len != 4)
        {
            return FAIL;
        }

        Geometry g = Geometry(data);
        if (g == geometry)
        {
            return NO_CHANGES;
        }

        geometry = g;
        return SUCCESS;
    }

    const std::vector<ScicosID>& getIn() const
    {
        return in;
    }

    void setIn(const std::vector<ScicosID>& in)
    {
        this->in = in;
    }

    const std::string& getInterfaceFunction() const
    {
        return interfaceFunction;
    }

    void setInterfaceFunction(const std::string& interfaceFunction)
    {
        this->interfaceFunction = interfaceFunction;
    }

    const std::vector<ScicosID>& getOut() const
    {
        return out;
    }

    void setOut(const std::vector<ScicosID>& out)
    {
        this->out = out;
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

    const std::string& getStyle() const
    {
        return style;
    }

    void setStyle(const std::string& style)
    {
        this->style = style;
    }
private:
    ScicosID parentDiagram;
    std::string interfaceFunction;
    Geometry geometry;
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
