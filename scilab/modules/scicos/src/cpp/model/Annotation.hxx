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

#ifndef ANNOTATION_HXX_
#define ANNOTATION_HXX_

#include <cassert>
#include <string>

#include "Model.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

class Annotation: public BaseObject
{
    friend class ::org_scilab_modules_scicos::Model;

private:
    Annotation(): BaseObject(ANNOTATION), parentDiagram(0), geometry(), description(), relatedTo(0) {};
    Annotation(const Annotation& o) : BaseObject(ANNOTATION), parentDiagram(o.parentDiagram), geometry(o.geometry), description(o.description), relatedTo(o.relatedTo) {};
    ~Annotation() {};

    const std::string& getDescription() const
    {
        return description;
    }

    update_status_t setDescription(const std::string& description)
    {
        if (this->description == description)
        {
            return NO_CHANGES;
        }

        this->description = description;
        return SUCCESS;
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

    ScicosID getParentDiagram() const
    {
        return parentDiagram;
    }

    update_status_t setParentDiagram(ScicosID parentDiagram)
    {
        if (this->parentDiagram == parentDiagram)
        {
            return NO_CHANGES;
        }

        this->parentDiagram = parentDiagram;
        return SUCCESS;
    }

    ScicosID getRelatedTo() const
    {
        return relatedTo;
    }

    update_status_t setRelatedTo(ScicosID relatedTo)
    {
        if (this->relatedTo == relatedTo)
        {
            return NO_CHANGES;
        }

        this->relatedTo = relatedTo;
        return SUCCESS;
    }

private:
    ScicosID parentDiagram;
    Geometry geometry;
    std::string description;
    ScicosID relatedTo;
};

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* ANNOTATION_HXX_ */
