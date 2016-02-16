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

#ifndef __SYMBOLIC_LIST_HXX__
#define __SYMBOLIC_LIST_HXX__

#include <iostream>

#include "GVN.hxx"
#include "TIType.hxx"

namespace analysis
{

/**
 * \class SymbolicList
 * \brief Manage a symbolic list
 */
class SymbolicList
{

    union Value
    {
        GVN::Value * gvnVal;
        double dval;

        Value() { }
        Value(GVN::Value * val) : gvnVal(val) { }
        Value(double val) : dval(val) { }
    };

    bool symbolic;

    Value start;
    Value step;
    Value end;

public:

    /**
     * \brief default constructor
     */
    SymbolicList() : symbolic(false), start(0.), step(0.), end(0.) { }

    /**
     * \brief constructor
     * \param _gvn the GVN to use
     * \param _start the starting value
     * \param _end the ending value
     */
    SymbolicList(GVN::Value * _start, GVN::Value * _step, GVN::Value * _end) : symbolic(true), start(_start), step(_step), end(_end) { }

    /**
     * \brief constructor
     * \param _gvn the GVN to use
     * \param _start the starting value
     * \param _end the ending value
     */
    SymbolicList(double _start, double _step, double _end) : symbolic(false), start(_start), step(_step), end(_end) { }

    /**
     * \brief constructor
     */
    SymbolicList(SymbolicList && sl) : symbolic(sl.symbolic), start(sl.start), step(sl.step), end(sl.end) { }

    inline SymbolicList & operator=(SymbolicList && sl)
    {
        symbolic = sl.symbolic;
        start = sl.start;
        step = sl.step;
        end = sl.end;

        return *this;
    }

    inline bool isSymbolic() const
    {
        return symbolic;
    }

    inline void setStart(GVN::Value * val)
    {
        start.gvnVal = val;
    }

    inline void setStep(GVN::Value * val)
    {
        step.gvnVal = val;
    }

    inline void setEnd(GVN::Value * val)
    {
        end.gvnVal = val;
    }

    inline GVN::Value * getStart() const
    {
        return start.gvnVal;
    }

    inline GVN::Value * getStep() const
    {
        return step.gvnVal;
    }

    inline GVN::Value * getEnd() const
    {
        return end.gvnVal;
    }

    inline double getStart(double) const
    {
        return start.dval;
    }

    inline double getStep(double) const
    {
        return step.dval;
    }

    inline double getEnd(double) const
    {
        return end.dval;
    }

    bool getType(GVN & gvn, TIType & type) const;
    void evalDollar(GVN & gvn, const GVN::Value * dollarVal);
    bool checkAsIndex(const GVN::Value * dim);

    static bool get(AnalysisVisitor & visitor, ast::ListExp & le, SymbolicList & sl);

    /**
     * \brief Overload of the << operator
     */
    friend inline std::wostream & operator<<(std::wostream & out, const SymbolicList & sl)
    {
        if (sl.symbolic)
        {
            out << *sl.start.gvnVal->poly << L" : " << *sl.step.gvnVal->poly << L" : " << *sl.end.gvnVal->poly;
        }
        else
        {
            out << sl.start.dval << L" : " << sl.step.dval << L" : " << sl.end.dval;
        }
        return out;
    }

    inline static GVN::Value * evalDollar(GVN & gvn, const GVN::Value * value, const GVN::Value * dollar, const GVN::Value * dollarVal)
    {
        if (value->poly->contains(dollar->value))
        {
            const MultivariatePolynomial & mp = value->poly->eval(std::pair<unsigned long long, const MultivariatePolynomial *>(dollar->value, dollarVal->poly));
            return gvn.getValue(mp);
        }

        return nullptr;
    }
};

} // namespace analysis

#endif // __SYMBOLIC_LIST_HXX__
