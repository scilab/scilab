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

struct Parameter
{
    // FIXME: list the possible parameters kind, name, and so on
    double foo;
};

// FIXME add more values there
enum SchedulingProperties
{
    DEP_U       = 1 << 0, //!< y=f(u)
    DEP_T       = 1 << 1, //!< y=f(x)
    BLOCKTYPE_H = 1 << 2, //!< y=f(u) but depends on t (if then else block)
};

struct Descriptor
{
    std::string functionName;
    int functionApi;

    // FIXME: should encode all possible values for dep_ut and blocktype
    int schedulingProperties;
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

    const Parameter& getParameters() const
    {
        return parameters;
    }

    void setParameters(const Parameter& parameters)
    {
        this->parameters = parameters;
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

    Parameter parameters;

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
