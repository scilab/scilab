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

#ifndef PORT_HXX_
#define PORT_HXX_

#include <string>
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
    Port() : BaseObject(PORT), dataType(0), sourceBlock(0), kind(UNDEF), implicit(false),
        style(), label(), firing(0), connectedSignals(std::vector<ScicosID> (1, 0)) {};
    Port(const Port& o) : BaseObject(PORT), dataType(o.dataType), sourceBlock(o.sourceBlock), kind(o.kind), implicit(o.implicit),
        style(o.style), label(o.label), firing(0), connectedSignals(o.connectedSignals) {};
    ~Port() {};

    const std::vector<ScicosID>& getConnectedSignals() const
    {
        return connectedSignals;
    }

    update_status_t setConnectedSignals(const std::vector<ScicosID>& connectedSignals)
    {
        if (this->connectedSignals == connectedSignals)
        {
            return NO_CHANGES;
        }

        this->connectedSignals = connectedSignals;
        return SUCCESS;
    }

    void getDataType(std::vector<int>& v) const
    {
        if (dataType == 0)
        {
            v.resize(3, 1);
        }
        else
        {
            v.resize(3);
            v[0] = dataType->rows;
            v[1] = dataType->columns;
            v[2] = dataType->datatype_id;
        }
    }

    update_status_t setDataType(Model* model, const std::vector<int>& v)
    {
        if (v.size() != 3)
        {
            return FAIL;
        }

        model::Datatype datatype = model::Datatype(v);
        if (this->dataType != 0 && *this->dataType == datatype)
        {
            return NO_CHANGES;
        }

        this->dataType = model->flyweight(datatype);
        return SUCCESS;
    }

    void getKind(int& k) const
    {
        k = kind;
    }

    update_status_t setKind(int k)
    {
        if (k < UNDEF || k > EOUT)
        {
            return FAIL;
        }

        if (k == kind)
        {
            return NO_CHANGES;
        }

        kind = static_cast<portKind>(k);
        return SUCCESS;
    }

    void getSourceBlock(ScicosID& sb) const
    {
        sb = sourceBlock;
    }

    update_status_t setSourceBlock(const ScicosID sb)
    {
        if (sb == this->sourceBlock)
        {
            return NO_CHANGES;
        }
        this->sourceBlock = sb;
        return SUCCESS;
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

    void getFiring(double& f) const
    {
        f = firing;
    }

    update_status_t setFiring(double firing)
    {
        if (firing == this->firing)
        {
            return NO_CHANGES;
        }
        this->firing = firing;
        return SUCCESS;
    }

private:
    Datatype* dataType;
    ScicosID sourceBlock;
    portKind kind;
    bool implicit;
    std::string style;
    std::string label;
    double firing;

    std::vector<ScicosID> connectedSignals;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* PORT_HXX_ */
