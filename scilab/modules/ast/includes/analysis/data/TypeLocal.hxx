/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __TYPE_LOCAL_HXX__
#define __TYPE_LOCAL_HXX__

#include <iostream>

#include "TIType.hxx"

namespace analysis
{

struct TypeLocal
{
    TIType::Type type;
    int rows;
    int cols;
    bool isAnInt;

    TypeLocal(const TIType::Type _type, const int _rows, const int _cols, const bool _isAnInt) : type(_type), rows(_rows), cols(_cols), isAnInt(_isAnInt) { }

    TypeLocal(const TypeLocal & tl) : type(tl.type), rows(tl.rows), cols(tl.cols), isAnInt(tl.isAnInt) { }

    inline bool operator<(const TypeLocal & R) const
    {
        return type < R.type || (type == R.type && (rows < R.rows || (rows == R.rows && (cols < R.cols || (cols == R.cols && (int)isAnInt < (int)R.isAnInt)))));
    }

    inline bool operator==(const TypeLocal & R) const
    {
        return type == R.type && rows == R.rows && cols == R.cols && isAnInt == R.isAnInt;
    }

    inline bool isScalar() const
    {
        return rows == 1 && cols == 1;
    }

    inline bool isKnown() const
    {
        return rows != -1;
    }

    inline int totalSize() const
    {
        return isKnown() ? (rows * cols) : -1;
    }

    inline static TypeLocal get(const TIType & type, const bool isAnInt = false)
    {
        if (type.isConstantDims())
        {
            return TypeLocal(type.type, type.rows.getConstant(), type.cols.getConstant(), isAnInt);
        }
        else
        {
            return TypeLocal(type.type, -1, -1, false);
        }
    }

    friend std::wostream & operator<<(std::wostream & out, const TypeLocal & tl)
    {
        out << TIType::toString(tl.type);
        if (tl.isAnInt)
        {
            out << L"(int64_t)";
        }
        out << L" ";
        if (tl.rows == -1 || tl.cols == -1)
        {
            out << L"?x?";
        }
        else
        {
            out << tl.rows << L'x' << tl.cols;
        }
        return out;
    }
};

} // namespace analysis

#endif // __TYPE_LOCAL_HXX__
