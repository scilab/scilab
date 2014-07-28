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

#ifndef PORT_HXX_
#define PORT_HXX_

#include <vector>

#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

enum portKind
{
    UNDEF, IN, OUT, EIN, EOUT
};

class Port: public BaseObject
{
    friend class ::org_scilab_modules_scicos::Model;

private:
    Port() : BaseObject(PORT), dataType(0), sourceBlock(0), kind(UNDEF), implicit(false), style(), label(), connectedSignals() {};
    Port(const Port& o) : BaseObject(PORT), dataType(o.dataType), sourceBlock(o.sourceBlock),
        kind(o.kind), implicit(o.implicit), style(o.style), label(o.label), connectedSignals(o.connectedSignals) {};
    ~Port() {};

    const std::vector<ScicosID>& getConnectedSignals() const
    {
        return connectedSignals;
    }

    void setConnectedSignals(const std::vector<ScicosID>& connectedSignals)
    {
        this->connectedSignals = connectedSignals;
    }

    Datatype* getDataType() const
    {
        return dataType;
    }

    void setDataType(Datatype* dataType)
    {
        this->dataType = dataType;
    }

    portKind getKind() const
    {
        return kind;
    }

    void setKind(portKind kind)
    {
        this->kind = kind;
    }

    ScicosID getSourceBlock() const
    {
        return sourceBlock;
    }

    void setSourceBlock(ScicosID sourceBlock)
    {
        this->sourceBlock = sourceBlock;
    }

    void getImplicit(bool& v) const
    {
        v = implicit;
    }

    update_status_t setImplicit(bool implicit)
    {
        if (implicit == this->implicit)
        {
            return NO_CHANGES;
        }
        this->implicit = implicit;
        return SUCCESS;
    }

    void getStyle(std::string& s) const
    {
        s = style;
    }

    update_status_t setStyle(const std::string& style)
    {
        if (style == this->style)
        {
            return NO_CHANGES;
        }
        this->style = style;
        return SUCCESS;
    }

    void getLabel(std::string& l) const
    {
        l = style;
    }

    update_status_t setLabel(const std::string& label)
    {
        if (label == this->label)
        {
            return NO_CHANGES;
        }
        this->label = label;
        return SUCCESS;
    }

private:
    Datatype* dataType;
    ScicosID sourceBlock;
    portKind kind;
    bool implicit;
    std::string style;
    std::string label;

    std::vector<ScicosID> connectedSignals;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* PORT_HXX_ */
