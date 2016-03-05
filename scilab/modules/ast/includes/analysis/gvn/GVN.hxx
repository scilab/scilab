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

#ifndef __GVN_HXX__
#define __GVN_HXX__

#include <string>
#include <list>
#include <map>
#include <unordered_map>

#include "MultivariatePolynomial.hxx"
#include "OpValue.hxx"
#include "symbol.hxx"
#include "dynlib_ast.h"

namespace analysis
{

/**
 * \class GVN
 * \brief GVN stands for Global Value Numbering
 *
 * This class implements a GVN but it is extended to track common expressions represented
 * by multivariate polynomials.
 * Symbolic calculus is allowed with integer values (such as matrix dimensions)
 *
 * All the pointers GVN::Value* returned by the GVN have the same lifetime than the GVN itself.
 * For information, the GVN::Value are stored in unordered_map (or multimap) or list.
 *
 */
class EXTERN_AST GVN
{

public:

    /**
     * \struct Value
     * \brief Represents a value and polynomial
     */
    struct Value
    {
        uint64_t value;
        const MultivariatePolynomial * poly;

        Value(const uint64_t _value) : value(_value), poly(nullptr) { }

        friend inline std::wostream & operator<<(std::wostream & out, const Value & v)
        {
            out << L"Value: " << v.value
                << L", Poly: ";
            if (v.poly)
            {
                out << *v.poly;
            }
            else
            {
                out << L"null";
            }
            return out;
        }

        inline bool operator==(const Value & R) const
        {
            return value == R.value;
        }

        inline bool operator!=(const Value & R) const
        {
            return value != R.value;
        }
    };

private:

    typedef std::unordered_map<OpValue, Value, OpValue::Hash, OpValue::Eq> MapValues;
    typedef std::unordered_map<int64_t, Value> MapInt64;
    typedef std::multimap<symbol::Symbol, Value> MapSymbols;
    typedef std::unordered_map<MultivariatePolynomial, Value *, MultivariatePolynomial::Hash, MultivariatePolynomial::Eq> MapPolys;
    typedef std::list<Value> ListValues;

    MapValues mapv;
    MapInt64 mapi64;
    MapSymbols maps;
    MapPolys mapp;
    ListValues list;

    uint64_t current;

public:

    // WARNING: current MUST be initialized with 0 and nothing else !
    // (because when mpoly are evaluated with a std::vector<MP> the index begins to 0)
    /**
     * \brief constructor
     */
    GVN() : current(0)
    {
    }

    /**
     * \brief Clear all the maps used in the GVN
     */
    void clear();

    /**
     * \brief Get current value
     */
    uint64_t getCurrentValue() const;

    /**
    * \brief Inserts a value associated with a polynomial
    * \param mp a polynomial
    * \param value a value
    */
    void insertValue(const MultivariatePolynomial & mp, Value & value);

    /**
     * \brief Associated a symbol with a polynomial
     * \param sym a symbol
     * \param mp a polynomial
     */
    void setValue(const symbol::Symbol & sym, const MultivariatePolynomial & mp);

    /**
     * \brief Associated a symbol with a Value
     * \param sym a symbol
     * \param LV a value
     */
    void setValue(const symbol::Symbol & sym, const Value & LV);

    /**
     * \brief Get a value
     * \return a Value
     */
    Value * getValue();

    /**
     * \brief Get a value associated with a polynomial
     * \param mp a polynomial
     * \return a Value
     */
    Value * getValue(const MultivariatePolynomial & mp);

    /**
     * \brief Get a value associated with a symbol
     * \param sym a symbol
     * \return a Value
     */
    Value * getValue(const symbol::Symbol & sym);

    /**
     * \brief Get a value associated with a symbol
     * \param sym a symbol
     * \return a Value
     */
    Value * getExistingValue(const symbol::Symbol & sym);

    /**
    * \brief Get a value associated with an integer
    * \param x an integer
    * \return a Value
    */
    Value * getValue(const int64_t x);

    /**
     * \brief Get a value associated with a double
     * \param x a double
     * \return a Value
     */
    Value * getValue(const double x);

    /**
     * \brief Get a value associated with an unary operation applyed to a value
     * \param kind the kind of the operation
     * \param LV a Value
     * \return a Value
     */
    Value * getValue(const OpValue::Kind kind, const Value & LV);

    /**
     * \brief Get a value associated with a binary operation applyed to two values
     * \param kind the kind of the operation
     * \param LV a Value (Left)
     * \param RV a Value (Right)
     * \return a Value
     */
    Value * getValue(const OpValue::Kind kind, const Value & LV, const Value & RV);

    /**
     * \brief Get a map containing association between symbol names and value (as ULL)
     * \return a map
     */
    std::map<std::wstring, uint64_t> getSymMap() const;

    /**
     * \brief Overload of the operator << for GVN
     */
    EXTERN_AST friend std::wostream & operator<<(std::wostream & out, const GVN & gvn);

private:

    /**
     * \brief Get a value associated with a polynomial which is the result of an operation
     * \param mp a polynomial
     * \param ov an operation
     * \return a Value
     */
    Value * getValue(const MultivariatePolynomial & mp, const OpValue & ov);

    /**
     * \brief Helper function to get the result of an unary operation applyed to the polynomial associated with a value
     * \param OPER the operation
     * \param LV a value
     * \param ov the operation kind
     * \return a Value
     */
    Value * getValue(MultivariatePolynomial(OPER)(const MultivariatePolynomial & mp), const Value & LV, const OpValue & ov);

    /**
     * \brief Helper function to get the result of a binary operation applyed to the polynomials associated with two values
     * \param OPER the operation
     * \param LV a value (Left)
     * \param RV a value (Right)
     * \param ov the operation kind
     * \return a Value
     */
    Value * getValue(MultivariatePolynomial(OPER)(const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP), const Value & LV, const Value & RV, const OpValue & ov);

};

} // namespace analysis

#endif // __GVN_HXX__
