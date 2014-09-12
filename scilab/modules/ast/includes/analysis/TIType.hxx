/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This file has been generated, so don't modify it by hand !!

#ifndef __TITYPE_HXX__
#define __TITYPE_HXX__

#include <iostream>

namespace analysis
{
struct TIType
{
    enum Type { EMPTY, BOOLEAN, COMPLEX, DOUBLE, INT16, INT32, INT64, INT8, POLYNOMIAL, STRING, SPARSE, UINT16, UINT32, UINT64, UINT8, UNKNOWN, COUNT = 16 };
    Type type;
    int rows;
    int cols;

    TIType() : type(Type::UNKNOWN),  rows(1), cols(1) { }
    TIType(Type _type) : type(_type), rows(1), cols(1) { }
    TIType(Type _type, int _rows, int _cols) : type(_type), rows(_rows), cols(_cols) { }

    inline bool isscalar() const
    {
        return type != EMPTY && rows == 1 && cols == 1;
    }

    inline bool isknown() const
    {
        return type != UNKNOWN;
    }

    inline bool operator==(const TIType & r) const
    {
        return type == r.type && rows == r.rows && cols == r.cols;
    }

    friend std::wostream & operator<<(std::wostream & out, const TIType & type)
    {
        switch (type.type)
        {
            case EMPTY :
                out << L"[]";
                break;
            case BOOLEAN :
                out << L"boolean";
                break;
            case COMPLEX :
                out << L"complex";
                break;
            case DOUBLE :
                out << L"double";
                break;
            case INT16 :
                out << L"int16";
                break;
            case INT32 :
                out << L"int32";
                break;
            case INT64 :
                out << L"int64";
                break;
            case INT8 :
                out << L"int8";
                break;
            case POLYNOMIAL :
                out << L"polynomial";
                break;
            case STRING :
                out << L"string";
                break;
            case SPARSE :
                out << L"sparse";
                break;
            case UINT16 :
                out << L"uint16";
                break;
            case UINT32 :
                out << L"uint32";
                break;
            case UINT64 :
                out << L"uint64";
                break;
            case UINT8 :
                out << L"uint8";
                break;
            case UNKNOWN :
                out << L"unknown";
        }

        if (type.type != EMPTY && type.type != UNKNOWN)
        {
            out << L"[" << type.rows << L", " << type.cols << L"]";
        }

        return out;
    }
};

} // namespace analysis

namespace std
{
// useful to be able to put TIType in unorderd_set for example.
template<>
struct hash<analysis::TIType>
{
    inline size_t operator()(const analysis::TIType & t) const
    {
        return t.type + (analysis::TIType::COUNT + 1) * (t.rows + 7057 * t.cols);
    }
};
} // namespace std

#endif // __TITYPE_HXX__
