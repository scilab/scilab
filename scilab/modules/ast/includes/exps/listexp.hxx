/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_LISTEXP_HXX
#define AST_LISTEXP_HXX

#include "exp.hxx"
#include <limits>

namespace ast
{

/** \brief Abstract an Operation Expression node.
**
** \b Example: 1:0.5:10 */
class ListExp : public Exp
{

public:
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Implicit list Expression node.
    ** \param location scanner position informations
    ** \param start the start point of the list
    ** \param step the step between each element
    ** \param end the end of the list (the max value)
    **
    ** \b Example: start:step:stop
    ** \li "start" is the start point of the list
    ** \li "step" is the step between each element
    ** \li "stop" is the end of the list (the max value)
    */
    ListExp (const Location& location,
             Exp& start, Exp& step, Exp& end, bool explicitStep = false)
        : Exp (location),
          _explicitStep(explicitStep)
    {
        start.setParent(this);
        step.setParent(this);
        end.setParent(this);
        _exps.push_back(&start);
        _exps.push_back(&step);
        _exps.push_back(&end);

        values[0] = std::numeric_limits<double>::quiet_NaN();
        values[1] = std::numeric_limits<double>::quiet_NaN();
        values[2] = std::numeric_limits<double>::quiet_NaN();

        is_values_int[0] = false;
        is_values_int[1] = false;
        is_values_int[2] = false;
    }

    /** \brief Destroy a Operation Expression node.
    **
    ** Delete left and right, see constructor. */
    virtual ~ListExp ()
    {
    }
    /** \} */

    virtual ListExp* clone()
    {
        ListExp* cloned = new ListExp(getLocation(), *getStart().clone(), *getStep().clone(), *getEnd().clone());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    /** \name Visitors entry point.
    ** \{ */
public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }
    /** \brief Accept a non-const visitor \a v. */
    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }
    /** \} */


    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the expression (read only) */
    const Exp& getStart () const
    {
        return *_exps[0];
    }
    /** \brief Return the expression (read and write) */
    Exp& getStart ()
    {
        return *_exps[0];
    }

    /** \brief Return the expression (read only) */
    const Exp& getStep () const
    {
        return *_exps[1];
    }
    /** \brief Return the expression (read and write) */
    Exp& getStep ()
    {
        return *_exps[1];
    }

    /** \brief Return the expression (read only) */
    const Exp& getEnd () const
    {
        return *_exps[2];
    }
    /** \brief Return the expression (read and write) */
    Exp& getEnd ()
    {
        return *_exps[2];
    }

    inline void setValues(double start, double step, double end)
    {
        values[0] = start;
        values[1] = step;
        values[2] = end;
    }

    inline const double * getValues() const
    {
        return values;
    }

    inline virtual bool is_list_exp() const
    {
        return true;
    }

    inline void set_values(double start, double step, double end)
    {
        values[0] = start;
        values[1] = step;
        values[2] = end;
    }

    inline const double * get_values() const
    {
        return values;
    }

    /** \brief Return if expression has explicit step defined */
    bool hasExplicitStep () const
    {
        return _explicitStep;
    }

    /** \} */
    virtual ExpType getType()
    {
        return LISTEXP;
    }
    inline bool isListExp() const
    {
        return true;
    }

protected:
    double values[3];
    bool is_values_int[3];
    /** \brief has list explicit step. */
    bool _explicitStep;
};

} // namespace ast

#endif // !AST_LISTEXP_HXX
