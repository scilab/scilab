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
        m_id(0), m_kind(k)
    {
    }
    BaseObject(const BaseObject& b) :
        m_id(0), m_kind(b.m_kind)
    {
    }
    virtual ~BaseObject() = default;

    inline bool operator<(BaseObject o) const
    {
        return m_id < o.m_id;
    }
    inline bool operator==(BaseObject o) const
    {
        return m_id == o.m_id;
    }

    inline ScicosID id() const
    {
        return m_id;
    }
    inline void id(ScicosID _id)
    {
        m_id = _id;
    }

    inline kind_t kind() const
    {
        return m_kind;
    }

private:
    /**
     * An id is used as a reference to the current object
     */
    ScicosID m_id;

    /**
     * Kind of the Object
     */
    const kind_t m_kind;
};

/** @defgroup utilities Shared utility classes
 * @{
 */

/*
 * Represent a graphical object
 */
struct Geometry
{
    double m_x;
    double m_y;
    double m_width;
    double m_height;

    Geometry() : m_x(0), m_y(0), m_width(20), m_height(20) {};
    Geometry(const Geometry& g) : m_x(g.m_x), m_y(g.m_y), m_width(g.m_width), m_height(g.m_height) {};
    Geometry(const std::vector<double>& v) : m_x(v[0]), m_y(v[1]), m_width(v[2]), m_height(v[3]) {};

    void fill(std::vector<double>& v) const
    {
        v.resize(4);
        v[0] = m_x;
        v[1] = m_y;
        v[2] = m_width;
        v[3] = m_height;
    }
    bool operator==(const Geometry& g) const
    {
        return m_x == g.m_x && m_y == g.m_y && m_width == g.m_width && m_height == g.m_height;
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
        m_refCount(0), m_datatype_id(d.m_datatype_id), m_rows(d.m_rows), m_columns(d.m_columns) {};
    Datatype(const std::vector<int>& v) :
        m_refCount(0), m_datatype_id(v[2]), m_rows(v[0]), m_columns(v[1]) {};

    // reference counter for the flyweight pattern
    int m_refCount;

    const int m_datatype_id;
    const int m_rows;
    const int m_columns;

    bool operator==(const Datatype& d) const
    {
        return m_datatype_id == d.m_datatype_id && m_rows == d.m_rows && m_columns == d.m_columns;
    }

    bool operator<(const Datatype& d) const
    {
        // Lexicographical order
        return m_datatype_id < d.m_datatype_id ||
               (m_datatype_id == d.m_datatype_id && m_rows < d.m_rows) ||
               (m_datatype_id == d.m_datatype_id && m_rows == d.m_rows && m_columns < d.m_columns);
    }
};

/** @}*/

} /* namespace model */
} /* namespace org_scilab_modules_scicos */

#endif /* BASEOBJECT_HXX_ */
