/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Calixte DENIZET
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

// This file has been generated, so don't modify it by hand !!

#ifndef __TITYPE_HXX__
#define __TITYPE_HXX__

#include <cstdint>
#include <functional>
#include <iostream>

#include "tools.hxx"
#include "gvn/GVN.hxx"
#include "gvn/SymbolicDimension.hxx"

namespace analysis
{
struct TIType
{
    static const std::wstring _boolean_, _ce_, _constant_, _fptr_, _function_, _int16_, _int32_, _int64_, _int8_, _library_, _list_, _mlist_, _polynomial_, _sparse_, _st_, _string_, _tlist_, _uint16_, _uint32_, _uint64_, _uint8_, _unknown_;

    enum Type { EMPTY = 0, BOOLEAN, COMPLEX, CELL, DOUBLE, FUNCTION, INT16, INT32, INT64, INT8, LIST, LIBRARY, MACRO, MACROFILE, MLIST, POLYNOMIAL, STRING, SPARSE, STRUCT, TLIST, UNKNOWN, UINT16, UINT32, UINT64, UINT8, COUNT };
    Type type;
    SymbolicDimension rows;
    SymbolicDimension cols;
    bool scalar;

    TIType(const Type _type = UNKNOWN) : type(_type), scalar(true) { }
    TIType(GVN & gvn) : type(UNKNOWN), rows(gvn, int64_t(0)), cols(gvn, int64_t(0)), scalar(false) { }
    TIType(GVN & gvn, const Type _type) : type(_type), rows(gvn, _type == EMPTY ? 0 : 1), cols(gvn, _type == EMPTY ? 0 : 1), scalar(_type != EMPTY) { }
    TIType(GVN & gvn, const Type _type, const int _rows, const int _cols) : type(_type), rows(gvn, _rows), cols(gvn, _cols), scalar(_rows == 1 && _cols == 1) { }
    TIType(GVN & /*gvn*/, Type _type, const SymbolicDimension & _rows, const SymbolicDimension & _cols) : type(_type), rows(_rows), cols(_cols), scalar(_rows == 1 && _cols == 1) { }
    TIType(GVN & gvn, const Type _type, const bool _scalar) : type(_type), rows(gvn, _scalar ? 1 : -1), cols(gvn, _scalar ? 1 : -1), scalar(_scalar) { }

    inline bool hasValidDims() const
    {
        return rows != -2 && cols != -2;
    }

    inline bool hasInvalidDims() const
    {
        return rows == -2;
    }

    inline TIType asUnknownMatrix()
    {
        return TIType(*rows.getGVN(), type, false);
    }

    inline bool isscalar() const
    {
        return scalar;
    }

    inline void invalidScalar()
    {
        scalar = rows == 1 && cols == 1;
    }

    inline bool iscomplex() const
    {
        return type == COMPLEX;
    }

    inline bool isreal() const
    {
        return type == DOUBLE;
    }

    inline bool isintegral() const
    {
        return type == BOOLEAN || type == INT8 || type == INT16 || type == INT32 || type == INT64 || type == UINT8 || type == UINT16 || type == UINT32 || type == UINT64;
    }

    inline bool ismatrix() const
    {
        return type != CELL && type != FUNCTION && type != LIST && type != LIBRARY && type != MACRO && type != MACROFILE && type != MLIST && type != STRUCT && type != TLIST && type != UNKNOWN;
    }

    inline bool issigned() const
    {
        return type != UINT8 && type != UINT16 && type != UINT32 && type != UINT64;
    }

    inline bool isfloating() const
    {
        return type == DOUBLE || type == COMPLEX;
    }

    inline bool isKnownDims() const
    {
        return rows.isValid() && cols.isValid();
    }

    inline bool isUnknownDims() const
    {
        return !isKnownDims();
    }

    inline bool isknown() const
    {
        return type != UNKNOWN;
    }

    inline bool isunknown() const
    {
        return type == UNKNOWN;
    }

    inline bool isConstantDims() const
    {
        return rows.isConstant() && cols.isConstant();
    }

    inline void swapDims()
    {
        GVN::Value * r = rows.getValue();
        rows.setValue(cols.getValue());
        cols.setValue(r);
    }

    inline std::size_t hashPureType() const
    {
        return isscalar() ? type : (type + TIType::COUNT + 1);
    }

    inline bool operator==(const TIType & r) const
    {
        return type == r.type && scalar == r.scalar && rows == r.rows && cols == r.cols;
    }

    inline bool operator!=(const TIType & r) const
    {
        return !(*this == r);
    }

    inline void merge(const TIType & type)
    {
        if (this->type == DOUBLE && type.type == COMPLEX)
        {
            this->type = COMPLEX;
        }
        else if ((this->type != COMPLEX || type.type != DOUBLE) && this->type != type.type)
        {
            this->type = UNKNOWN;
            rows.setValue(int64_t(0));
            cols.setValue(int64_t(0));
        }
        else if ((!scalar || !type.scalar) && (rows != type.rows || cols != type.cols))
        {
            if (rows != type.rows)
            {
                rows.setValue(rows.getGVN()->getValue());
            }
            if (cols != type.cols)
            {
                cols.setValue(cols.getGVN()->getValue());
            }
            scalar = false;
        }
        /*else if ((!scalar || !type.scalar) && (rows != type.rows || cols != type.cols))
        {
        rows.invalid();
        cols.invalid();
        scalar = false;
        }*/
    }

    template<typename> static Type getTI();

    inline std::string get_mangling() const
    {
        const bool kd = isKnownDims();
        switch (type)
        {
            case EMPTY :
                return "E";
            case BOOLEAN :
                return kd ? (scalar ? "Sb" : "Mb") : "Ub";
            case COMPLEX :
                return kd ? (scalar ? "Sc" : "Mc") : "Uc";
            case CELL :
                return kd ? (scalar ? "Sce" : "Mce") : "Uce";
            case DOUBLE :
                return kd ? (scalar ? "Sd" : "Md") : "Ud";
            case FUNCTION :
                return kd ? (scalar ? "Sfn" : "Mfn") : "Ufn";
            case INT16 :
                return kd ? (scalar ? "Si16" : "Mi16") : "Ui16";
            case INT32 :
                return kd ? (scalar ? "Si32" : "Mi32") : "Ui32";
            case INT64 :
                return kd ? (scalar ? "Si64" : "Mi64") : "Ui64";
            case INT8 :
                return kd ? (scalar ? "Si8" : "Mi8") : "Ui8";
            case LIST :
                return kd ? (scalar ? "Sl" : "Ml") : "Ul";
            case LIBRARY :
                return kd ? (scalar ? "Slb" : "Mlb") : "Ulb";
            case MACRO :
                return kd ? (scalar ? "Sm" : "Mm") : "Um";
            case MACROFILE :
                return kd ? (scalar ? "Smf" : "Mmf") : "Umf";
            case MLIST :
                return kd ? (scalar ? "Sml" : "Mml") : "Uml";
            case POLYNOMIAL :
                return kd ? (scalar ? "Sp" : "Mp") : "Up";
            case STRING :
                return kd ? (scalar ? "Ss" : "Ms") : "Us";
            case SPARSE :
                return kd ? (scalar ? "Ssp" : "Msp") : "Usp";
            case STRUCT :
                return kd ? (scalar ? "Sst" : "Mst") : "Ust";
            case TLIST :
                return kd ? (scalar ? "Stl" : "Mtl") : "Utl";
            case UNKNOWN :
                return kd ? (scalar ? "Su" : "Mu") : "Uu";
            case UINT16 :
                return kd ? (scalar ? "Sui16" : "Mui16") : "Uui16";
            case UINT32 :
                return kd ? (scalar ? "Sui32" : "Mui32") : "Uui32";
            case UINT64 :
                return kd ? (scalar ? "Sui64" : "Mui64") : "Uui64";
            case UINT8 :
                return kd ? (scalar ? "Sui8" : "Mui8") : "Uui8";
            default :
                return "??";
        }
    }

    inline std::wstring get_manglingW() const
    {
        const bool kd = isKnownDims();
        switch (type)
        {
            case EMPTY :
                return L"E";
            case BOOLEAN :
                return kd ? (scalar ? L"Sb" : L"Mb") : L"Ub";
            case COMPLEX :
                return kd ? (scalar ? L"Sc" : L"Mc") : L"Uc";
            case CELL :
                return kd ? (scalar ? L"Sce" : L"Mce") : L"Uce";
            case DOUBLE :
                return kd ? (scalar ? L"Sd" : L"Md") : L"Ud";
            case FUNCTION :
                return kd ? (scalar ? L"Sfn" : L"Mfn") : L"Ufn";
            case INT16 :
                return kd ? (scalar ? L"Si16" : L"Mi16") : L"Ui16";
            case INT32 :
                return kd ? (scalar ? L"Si32" : L"Mi32") : L"Ui32";
            case INT64 :
                return kd ? (scalar ? L"Si64" : L"Mi64") : L"Ui64";
            case INT8 :
                return kd ? (scalar ? L"Si8" : L"Mi8") : L"Ui8";
            case LIST :
                return kd ? (scalar ? L"Sl" : L"Ml") : L"Ul";
            case LIBRARY :
                return kd ? (scalar ? L"Slb" : L"Mlb") : L"Ulb";
            case MACRO :
                return kd ? (scalar ? L"Sm" : L"Mm") : L"Um";
            case MACROFILE :
                return kd ? (scalar ? L"Smf" : L"Mmf") : L"Umf";
            case MLIST :
                return kd ? (scalar ? L"Sml" : L"Mml") : L"Uml";
            case POLYNOMIAL :
                return kd ? (scalar ? L"Sp" : L"Mp") : L"Up";
            case STRING :
                return kd ? (scalar ? L"Ss" : L"Ms") : L"Us";
            case SPARSE :
                return kd ? (scalar ? L"Ssp" : L"Msp") : L"Usp";
            case STRUCT :
                return kd ? (scalar ? L"Sst" : L"Mst") : L"Ust";
            case TLIST :
                return kd ? (scalar ? L"Stl" : L"Mtl") : L"Utl";
            case UNKNOWN :
                return kd ? (scalar ? L"Su" : L"Mu") : L"Uu";
            case UINT16 :
                return kd ? (scalar ? L"Sui16" : L"Mui16") : L"Uui16";
            case UINT32 :
                return kd ? (scalar ? L"Sui32" : L"Mui32") : L"Uui32";
            case UINT64 :
                return kd ? (scalar ? L"Sui64" : L"Mui64") : L"Uui64";
            case UINT8 :
                return kd ? (scalar ? L"Sui8" : L"Mui8") : L"Uui8";
            default :
                return L"??";
        }
    }

    inline static std::string get_mangling(const TIType::Type ty, const bool scalar)
    {
        switch (ty)
        {
            case EMPTY :
                return "E";
            case BOOLEAN :
                return scalar ? "Sb" : "Mb";
            case COMPLEX :
                return scalar ? "Sc" : "Mc";
            case CELL :
                return scalar ? "Sce" : "Mce";
            case DOUBLE :
                return scalar ? "Sd" : "Md";
            case FUNCTION :
                return scalar ? "Sfn" : "Mfn";
            case INT16 :
                return scalar ? "Si16" : "Mi16";
            case INT32 :
                return scalar ? "Si32" : "Mi32";
            case INT64 :
                return scalar ? "Si64" : "Mi64";
            case INT8 :
                return scalar ? "Si8" : "Mi8";
            case LIST :
                return scalar ? "Sl" : "Ml";
            case LIBRARY :
                return scalar ? "Slb" : "Mlb";
            case MACRO :
                return scalar ? "Sm" : "Mm";
            case MACROFILE :
                return scalar ? "Smf" : "Mmf";
            case MLIST :
                return scalar ? "Sml" : "Mml";
            case POLYNOMIAL :
                return scalar ? "Sp" : "Mp";
            case STRING :
                return scalar ? "Ss" : "Ms";
            case SPARSE :
                return scalar ? "Ssp" : "Msp";
            case STRUCT :
                return scalar ? "Sst" : "Mst";
            case TLIST :
                return scalar ? "Stl" : "Mtl";
            case UNKNOWN :
                return scalar ? "Su" : "Mu";
            case UINT16 :
                return scalar ? "Sui16" : "Mui16";
            case UINT32 :
                return scalar ? "Sui32" : "Mui32";
            case UINT64 :
                return scalar ? "Sui64" : "Mui64";
            case UINT8 :
                return scalar ? "Sui8" : "Mui8";
            default :
                return "??";
        }
    }

    inline static std::wstring get_manglingW(const TIType::Type ty, const bool scalar)
    {
        switch (ty)
        {
            case EMPTY :
                return L"E";
            case BOOLEAN :
                return scalar ? L"Sb" : L"Mb";
            case COMPLEX :
                return scalar ? L"Sc" : L"Mc";
            case CELL :
                return scalar ? L"Sce" : L"Mce";
            case DOUBLE :
                return scalar ? L"Sd" : L"Md";
            case FUNCTION :
                return scalar ? L"Sfn" : L"Mfn";
            case INT16 :
                return scalar ? L"Si16" : L"Mi16";
            case INT32 :
                return scalar ? L"Si32" : L"Mi32";
            case INT64 :
                return scalar ? L"Si64" : L"Mi64";
            case INT8 :
                return scalar ? L"Si8" : L"Mi8";
            case LIST :
                return scalar ? L"Sl" : L"Ml";
            case LIBRARY :
                return scalar ? L"Slb" : L"Mlb";
            case MACRO :
                return scalar ? L"Sm" : L"Mm";
            case MACROFILE :
                return scalar ? L"Smf" : L"Mmf";
            case MLIST :
                return scalar ? L"Sml" : L"Mml";
            case POLYNOMIAL :
                return scalar ? L"Sp" : L"Mp";
            case STRING :
                return scalar ? L"Ss" : L"Ms";
            case SPARSE :
                return scalar ? L"Ssp" : L"Msp";
            case STRUCT :
                return scalar ? L"Sst" : L"Mst";
            case TLIST :
                return scalar ? L"Stl" : L"Mtl";
            case UNKNOWN :
                return scalar ? L"Su" : L"Mu";
            case UINT16 :
                return scalar ? L"Sui16" : L"Mui16";
            case UINT32 :
                return scalar ? L"Sui32" : L"Mui32";
            case UINT64 :
                return scalar ? L"Sui64" : L"Mui64";
            case UINT8 :
                return scalar ? L"Sui8" : L"Mui8";
            default :
                return L"??";
        }
    }

    inline static std::string get_unary_mangling(const std::string & pre, const TIType & l)
    {
        return pre + "_" + l.get_mangling();
    }

    inline static std::wstring get_unary_manglingW(const std::wstring & pre, const TIType & l)
    {
        return pre + L"_" + l.get_manglingW();
    }

    inline static std::string get_binary_mangling(const std::string & pre, const TIType & l, const TIType & r)
    {
        return pre + "_" + l.get_mangling() + r.get_mangling();
    }

    inline static std::string get_mangling(const std::string & pre, const std::vector<TIType> & types)
    {
        std::string s(pre);
        s += "_";
        for (std::vector<TIType>::const_iterator i = types.begin(), end = types.end(); i != end; ++i)
        {
            s += i->get_mangling();
        }
        return s;
    }

    inline static unsigned int get_base_size(const Type type)
    {
        switch (type)
        {
            case EMPTY :
                return sizeof(double);
            case BOOLEAN :
                return sizeof(int);
            case DOUBLE :
                return sizeof(double);
            case INT8 :
                return sizeof(int8_t);
            case INT16 :
                return sizeof(int16_t);
            case INT32 :
                return sizeof(int32_t);
            case INT64 :
                return sizeof(int64_t);
            case UINT8 :
                return sizeof(uint8_t);
            case UINT16 :
                return sizeof(uint16_t);
            case UINT32 :
                return sizeof(uint32_t);
            case UINT64 :
                return sizeof(uint64_t);
            default :
                return 0;
        }
    }

    inline static std::wstring toString(Type t)
    {
        switch (t)
        {
            case EMPTY :
                return L"[]";
            case BOOLEAN :
                return L"boolean";
            case COMPLEX :
                return L"complex";
            case CELL :
                return L"cell";
            case DOUBLE :
                return L"double";
            case FUNCTION :
                return L"function";
            case INT16 :
                return L"int16";
            case INT32 :
                return L"int32";
            case INT64 :
                return L"int64";
            case INT8 :
                return L"int8";
            case LIST :
                return L"list";
            case LIBRARY :
                return L"library";
            case MACRO :
                return L"macro";
            case MACROFILE :
                return L"macrofile";
            case MLIST :
                return L"mlist";
            case POLYNOMIAL :
                return L"polynomial";
            case STRING :
                return L"string";
            case SPARSE :
                return L"sparse";
            case STRUCT :
                return L"struct";
            case TLIST :
                return L"tlist";
            case UNKNOWN :
                return L"unknown";
            case UINT16 :
                return L"uint16";
            case UINT32 :
                return L"uint32";
            case UINT64 :
                return L"uint64";
            case UINT8 :
                return L"uint8";
            default :
                return L"unknown";
        }
    }

    inline const std::wstring & getScilabString() const
    {
        return getScilabString(type);
    }

    inline static const std::wstring & getScilabString(Type t)
    {
        switch (t)
        {
            case EMPTY :
                return _constant_;
            case BOOLEAN :
                return _boolean_;
            case COMPLEX :
                return _constant_;
            case CELL :
                return _ce_;
            case DOUBLE :
                return _constant_;
            case FUNCTION :
                return _fptr_;
            case INT16 :
                return _int16_;
            case INT32 :
                return _int32_;
            case INT64 :
                return _int64_;
            case INT8 :
                return _int8_;
            case LIST :
                return _list_;
            case LIBRARY :
                return _library_;
            case MACRO :
                return _function_;
            case MACROFILE :
                return _function_;
            case MLIST :
                return _mlist_;
            case POLYNOMIAL :
                return _polynomial_;
            case STRING :
                return _string_;
            case SPARSE :
                return _sparse_;
            case STRUCT :
                return _st_;
            case TLIST :
                return _tlist_;
            case UNKNOWN :
                return _unknown_;
            case UINT16 :
                return _uint16_;
            case UINT32 :
                return _uint32_;
            case UINT64 :
                return _uint64_;
            case UINT8 :
                return _uint8_;
            default :
                return _unknown_;
        }
    }

    inline int getScilabCode() const
    {
        return getScilabCode(type);
    }

    inline static int getScilabCode(Type t)
    {
        switch (t)
        {
            case EMPTY :
                return 1;
            case BOOLEAN :
                return 4;
            case COMPLEX :
                return 1;
            case CELL :
                return 17;
            case DOUBLE :
                return 1;
            case FUNCTION :
                return 130;
            case INT16 :
                return 8;
            case INT32 :
                return 8;
            case INT64 :
                return 8;
            case INT8 :
                return 8;
            case LIST :
                return 15;
            case LIBRARY :
                return 14;
            case MACRO :
                return 11;
            case MACROFILE :
                return 13;
            case MLIST :
                return 17;
            case POLYNOMIAL :
                return 2;
            case STRING :
                return 10;
            case SPARSE :
                return 5;
            case STRUCT :
                return 17;
            case TLIST :
                return 16;
            case UNKNOWN :
                return -1;
            case UINT16 :
                return 8;
            case UINT32 :
                return 8;
            case UINT64 :
                return 8;
            case UINT8 :
                return 8;
            default :
                return -1;
        }
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
            case CELL :
                out << L"cell";
                break;
            case DOUBLE :
                out << L"double";
                break;
            case FUNCTION :
                out << L"function";
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
            case LIST :
                out << L"list";
                break;
            case LIBRARY :
                out << L"library";
                break;
            case MACRO :
                out << L"macro";
                break;
            case MACROFILE :
                out << L"macrofile";
                break;
            case MLIST :
                out << L"mlist";
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
            case STRUCT :
                out << L"struct";
                break;
            case TLIST :
                out << L"tlist";
                break;
            case UNKNOWN :
                out << L"unknown";
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
            default :
                break;
        }

        if (type.type != EMPTY && type.type != UNKNOWN)
        {
            if (type.isUnknownDims())
            {
                out << L"[?, ?]";
            }
            else
            {
                out << L"[" << type.rows << L", " << type.cols << L"]";
            }
        }

        return out;
    }
};

template<> inline TIType::Type TIType::getTI<bool>()
{
    return TIType::BOOLEAN;
}
template<> inline TIType::Type TIType::getTI<double>()
{
    return TIType::DOUBLE;
}
template<> inline TIType::Type TIType::getTI<int16_t>()
{
    return TIType::INT16;
}
template<> inline TIType::Type TIType::getTI<int32_t>()
{
    return TIType::INT32;
}
template<> inline TIType::Type TIType::getTI<int64_t>()
{
    return TIType::INT64;
}
template<> inline TIType::Type TIType::getTI<int8_t>()
{
    return TIType::INT8;
}
template<> inline TIType::Type TIType::getTI<uint16_t>()
{
    return TIType::UINT16;
}
template<> inline TIType::Type TIType::getTI<uint32_t>()
{
    return TIType::UINT32;
}
template<> inline TIType::Type TIType::getTI<uint64_t>()
{
    return TIType::UINT64;
}
template<> inline TIType::Type TIType::getTI<uint8_t>()
{
    return TIType::UINT8;
}

} // namespace analysis

namespace std
{
// useful to be able to put TIType in unorderd_set for example.
template<>
struct hash<analysis::TIType>
{
    inline size_t operator()(const analysis::TIType & /*t*/) const
    {
        return 0;//tools::hash_combine(t.type, t.rows, t.cols);
    }
};
} // namespace std

#endif // __TITYPE_HXX__
