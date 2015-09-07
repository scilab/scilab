/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Calixte DENIZET
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
        TIType(GVN & gvn) : type(UNKNOWN), rows(gvn, 0.), cols(gvn, 0.), scalar(false) { }
        TIType(GVN & gvn, const Type _type) : type(_type), rows(gvn, _type == EMPTY ? 0 : 1), cols(gvn, _type == EMPTY ? 0 : 1), scalar(_type != EMPTY) { }
	TIType(GVN & gvn, const Type _type, const int _rows, const int _cols) : type(_type), rows(gvn, _rows), cols(gvn, _cols), scalar(_rows == 1 && _cols == 1) { }
	TIType(GVN & gvn, Type _type, const SymbolicDimension & _rows, const SymbolicDimension & _cols) : type(_type), rows(_rows), cols(_cols), scalar(_rows == 1 && _cols == 1) { }
	TIType(GVN & gvn, const Type _type, const bool _scalar) : type(_type), rows(gvn, _scalar ? 1. : -1.), cols(gvn, _scalar ? 1. : -1.), scalar(_scalar) { }

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
		rows.setValue(0.);
		cols.setValue(0.);
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
                return kd ? (scalar ? "S_b" : "M_b") : "U_b";
            case COMPLEX :
                return kd ? (scalar ? "S_c" : "M_c") : "U_c";
            case CELL :
                return kd ? (scalar ? "S_ce" : "M_ce") : "U_ce";
            case DOUBLE :
                return kd ? (scalar ? "S_d" : "M_d") : "U_d";
            case FUNCTION :
                return kd ? (scalar ? "S_fn" : "M_fn") : "U_fn";
            case INT16 :
                return kd ? (scalar ? "S_i16" : "M_i16") : "U_i16";
            case INT32 :
                return kd ? (scalar ? "S_i32" : "M_i32") : "U_i32";
            case INT64 :
                return kd ? (scalar ? "S_i64" : "M_i64") : "U_i64";
            case INT8 :
                return kd ? (scalar ? "S_i8" : "M_i8") : "U_i8";
            case LIST :
                return kd ? (scalar ? "S_l" : "M_l") : "U_l";
            case LIBRARY :
                return kd ? (scalar ? "S_lb" : "M_lb") : "U_lb";
            case MACRO :
                return kd ? (scalar ? "S_m" : "M_m") : "U_m";
            case MACROFILE :
                return kd ? (scalar ? "S_mf" : "M_mf") : "U_mf";
            case MLIST :
                return kd ? (scalar ? "S_ml" : "M_ml") : "U_ml";
            case POLYNOMIAL :
                return kd ? (scalar ? "S_p" : "M_p") : "U_p";
            case STRING :
                return kd ? (scalar ? "S_s" : "M_s") : "U_s";
            case SPARSE :
                return kd ? (scalar ? "S_sp" : "M_sp") : "U_sp";
            case STRUCT :
                return kd ? (scalar ? "S_st" : "M_st") : "U_st";
            case TLIST :
                return kd ? (scalar ? "S_tl" : "M_tl") : "U_tl";
            case UNKNOWN :
                return kd ? (scalar ? "S_u" : "M_u") : "U_u";
            case UINT16 :
                return kd ? (scalar ? "S_ui16" : "M_ui16") : "U_ui16";
            case UINT32 :
                return kd ? (scalar ? "S_ui32" : "M_ui32") : "U_ui32";
            case UINT64 :
                return kd ? (scalar ? "S_ui64" : "M_ui64") : "U_ui64";
            case UINT8 :
                return kd ? (scalar ? "S_ui8" : "M_ui8") : "U_ui8";
            default :
                return "??";
            }
        }

        inline static std::string get_mangling(const TIType::Type ty, const bool scalar)
        {
            switch (ty)
            {
            case EMPTY :
                return "E";
            case BOOLEAN :
                return scalar ? "S_b" : "M_b";
            case COMPLEX :
                return scalar ? "S_c" : "M_c";
            case CELL :
                return scalar ? "S_ce" : "M_ce";
            case DOUBLE :
                return scalar ? "S_d" : "M_d";
            case FUNCTION :
                return scalar ? "S_fn" : "M_fn";
            case INT16 :
                return scalar ? "S_i16" : "M_i16";
            case INT32 :
                return scalar ? "S_i32" : "M_i32";
            case INT64 :
                return scalar ? "S_i64" : "M_i64";
            case INT8 :
                return scalar ? "S_i8" : "M_i8";
            case LIST :
                return scalar ? "S_l" : "M_l";
            case LIBRARY :
                return scalar ? "S_lb" : "M_lb";
            case MACRO :
                return scalar ? "S_m" : "M_m";
            case MACROFILE :
                return scalar ? "S_mf" : "M_mf";
            case MLIST :
                return scalar ? "S_ml" : "M_ml";
            case POLYNOMIAL :
                return scalar ? "S_p" : "M_p";
            case STRING :
                return scalar ? "S_s" : "M_s";
            case SPARSE :
                return scalar ? "S_sp" : "M_sp";
            case STRUCT :
                return scalar ? "S_st" : "M_st";
            case TLIST :
                return scalar ? "S_tl" : "M_tl";
            case UNKNOWN :
                return scalar ? "S_u" : "M_u";
            case UINT16 :
                return scalar ? "S_ui16" : "M_ui16";
            case UINT32 :
                return scalar ? "S_ui32" : "M_ui32";
            case UINT64 :
                return scalar ? "S_ui64" : "M_ui64";
            case UINT8 :
                return scalar ? "S_ui8" : "M_ui8";
            default :
                return "??";
            }
        }

        inline static std::string get_unary_mangling(const std::string & pre, const TIType & l)
        {
            return pre + "_" + l.get_mangling();
        }

        inline static std::string get_binary_mangling(const std::string & pre, const TIType & l, const TIType & r)
        {
            return pre + "_" + l.get_mangling() + "_" + r.get_mangling();
        }

        inline static std::string get_mangling(const std::string & pre, const std::vector<TIType> & types)
        {
	    std::string s(pre);
	    for (std::vector<TIType>::const_iterator i = types.begin(), end = types.end(); i != end; ++i)
	    {
		s += "_" + i->get_mangling();
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

    template<> inline TIType::Type TIType::getTI<bool>() { return TIType::BOOLEAN; }
    template<> inline TIType::Type TIType::getTI<double>() { return TIType::DOUBLE; }
    template<> inline TIType::Type TIType::getTI<int16_t>() { return TIType::INT16; }
    template<> inline TIType::Type TIType::getTI<int32_t>() { return TIType::INT32; }
    template<> inline TIType::Type TIType::getTI<int64_t>() { return TIType::INT64; }
    template<> inline TIType::Type TIType::getTI<int8_t>() { return TIType::INT8; }
    template<> inline TIType::Type TIType::getTI<uint16_t>() { return TIType::UINT16; }
    template<> inline TIType::Type TIType::getTI<uint32_t>() { return TIType::UINT32; }
    template<> inline TIType::Type TIType::getTI<uint64_t>() { return TIType::UINT64; }
    template<> inline TIType::Type TIType::getTI<uint8_t>() { return TIType::UINT8; }

} // namespace analysis

namespace std
{
    // useful to be able to put TIType in unorderd_set for example.
    template<>
    struct hash<analysis::TIType>
    {
	inline size_t operator()(const analysis::TIType & t) const
	    {
		return 0;//tools::hash_combine(t.type, t.rows, t.cols);
	    }
    };
} // namespace std

#endif // __TITYPE_HXX__