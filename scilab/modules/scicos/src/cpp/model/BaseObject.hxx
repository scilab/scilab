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

#ifndef BASEOBJECT_HXX_
#define BASEOBJECT_HXX_

#include "utilities.hxx"

namespace org_scilab_modules_scicos
{
namespace model
{

class BaseObject
{
public:
    BaseObject(kind_t k) :
        _id(0), _kind(k)
    {
    }
    BaseObject(const BaseObject& b) :
        _id(0), _kind(b._kind)
    {
    }

    inline bool operator<(BaseObject o)
    {
        return _id < o._id;
    }
    inline bool operator==(BaseObject o)
    {
        return _id == o._id;
    }

    inline ScicosID id()
    {
        return _id;
    }
    inline void id(ScicosID id)
    {
        _id = id;
    }

    inline kind_t kind()
    {
        return _kind;
    }

private:
    /**
     * An id is used as a reference to the current object
     */
    ScicosID _id;

    /**
     * Kind of the Object
     */
    const kind_t _kind;
};

/** @defgroup utilities Shared utility classes
 * @{
 */

/*
 * Represent a graphical object
 */
struct Geometry
{
    double x;
    double y;
    double width;
    double height;

    Geometry() : x(0), y(0), width(0), height(0) {};
    Geometry(const Geometry& g) : x(g.x), y(g.y), width(g.width), height(g.height) {};
    Geometry(double* data) : x(data[0]), y(data[1]), width(data[2]), height(data[3]) {};
    double* copy() const
    {
        double* d = new double[4];
        d[0] = x;
        d[1] = y;
        d[2] = width;
        d[3] = height;
        return d;
    }
    bool operator==(const Geometry& g) const
    {
        return x == g.x && y == g.y && width == g.width && height == g.height;
    }
};

/**
 * Per port type descriptor
 *
 * FIXME: should reuse Scilab datatypes descriptors
 */
struct Datatype
{
public:
    Datatype(unsigned int datatype, unsigned int r, unsigned c) :
        datatype_id(datatype), rows(r), collumns(c)
    {
    }

    const unsigned int datatype_id;
    const unsigned int rows;
    const unsigned int collumns;

};

/** @}*/

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* BASEOBJECT_HXX_ */
