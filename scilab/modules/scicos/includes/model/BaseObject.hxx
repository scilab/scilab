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

#ifndef BASEOBJECT_HXX_
#define BASEOBJECT_HXX_

#include <vector>

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

    inline bool operator<(BaseObject o) const
    {
        return _id < o._id;
    }
    inline bool operator==(BaseObject o) const
    {
        return _id == o._id;
    }

    inline ScicosID id() const
    {
        return _id;
    }
    inline void id(ScicosID id)
    {
        _id = id;
    }

    inline kind_t kind() const
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

    Geometry() : x(0), y(0), width(20), height(20) {};
    Geometry(const Geometry& g) : x(g.x), y(g.y), width(g.width), height(g.height) {};
    Geometry(const std::vector<double>& v) : x(v[0]), y(v[1]), width(v[2]), height(v[3]) {};

    void fill(std::vector<double>& v) const
    {
        v.resize(4);
        v[0] = x;
        v[1] = y;
        v[2] = width;
        v[3] = height;
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
    Datatype(const Datatype& d) :
        refCount(0), datatype_id(d.datatype_id), rows(d.rows), columns(d.columns) {};
    Datatype(const std::vector<int>& v) :
        refCount(0), datatype_id(v[2]), rows(v[0]), columns(v[1]) {};

    // reference counter for the flyweight pattern
    int refCount;

    const int datatype_id;
    const int rows;
    const int columns;

    bool operator==(const Datatype& d) const
    {
        return datatype_id == d.datatype_id && rows == d.rows && columns == d.columns;
    }

    bool operator<(const Datatype& d) const
    {
        // Lexicographical order
        return datatype_id < d.datatype_id ||
               (datatype_id == d.datatype_id && rows < d.rows) ||
               (datatype_id == d.datatype_id && rows == d.rows && columns < d.columns);
    }
};

/** @}*/

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* BASEOBJECT_HXX_ */
