/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
** \file exp.hxx
** Define the Expression mother class.
*/

#ifndef AST_EXP_HXX
#define AST_EXP_HXX

#include <list>

#include "ast.hxx"

namespace ast
{

/** \brief Abstract an Expression node. */
class Exp : public Ast
{

    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Expression node.
    ** \param location scanner position informations */
    Exp (const Location& location)
        : Ast (location),
          _verbose(false),
          _bBreak(false),
          _bBreakable(false),
          _bReturn(false),
          _bReturnable(false),
          _bContinue(false),
          _bContinuable(false)
    {
    }
    /** \brief Destroys an Expression node. */
    virtual ~Exp ()
    {
    }
    /** \} */

    virtual Exp* clone() = 0;

public:
    /** \brief Return if an expression should be displayed or not. */
    inline void mute(void)
    {
        _verbose = false;
    }

    /** \brief Return if an expression should be displayed or not. */
    inline void setVerbose(bool verbose)
    {
        _verbose = verbose;
    }

    /** \brief Return if an expression should be displayed or not. */
    inline virtual bool isVerbose(void) const
    {
        return _verbose;
    }

    inline void setBreak(void)
    {
        _bBreak = true;
    }

    inline void resetBreak(void)
    {
        _bBreak = false;
    }

    inline virtual bool isBreak(void) const
    {
        return _bBreak;
    }

    inline void setBreakable(void)
    {
        _bBreakable = true;
    }

    inline void resetBreakable(void)
    {
        _bBreakable = false;
    }

    inline virtual bool isBreakable(void) const
    {
        return _bBreakable;
    }

    inline void setReturn(void)
    {
        _bReturn = true;
    }

    inline void resetReturn(void)
    {
        _bReturn = false;
    }

    inline virtual bool isReturn(void) const
    {
        return _bReturn;
    }

    inline void setReturnable(void)
    {
        _bReturnable = true;
    }

    inline void resetReturnable(void)
    {
        _bReturnable = false;
    }

    inline virtual bool isReturnable(void) const
    {
        return _bReturnable;
    }

    inline void setContinue(void)
    {
        _bContinue = true;
    }

    inline void resetContinue(void)
    {
        _bContinue = false;
    }

    inline virtual bool isContinue(void) const
    {
        return _bContinue;
    }

    inline void setContinuable(void)
    {
        _bContinuable = true;
    }

    inline void resetContinuable(void)
    {
        _bContinuable = false;
    }

    inline virtual bool isContinuable(void) const
    {
        return _bContinuable;
    }

    //manage derived exp
    enum ExpType
    {
        SIMPLEVAR,
        DOLLARVAR,
        COLONVAR,
        ARRAYLISTVAR,
        DOUBLEEXP,
        BOOLEXP,
        STRINGEXP,
        COMMENTEXP,
        NILEXP,
        CALLEXP,
        CELLCALLEXP,
        OPEXP,
        LOGICALOPEXP,
        ASSIGNEXP,
        IFEXP,
        WHILEEXP,
        FOREXP,
        BREAKEXP,
        CONTINUEEXP,
        TRYCATCHEXP,
        SELECTEXP,
        CASEEXP,
        RETURNEXP,
        FIELDEXP,
        NOTEXP,
        TRANSPOSEEXP,
        MATRIXEXP,
        MATRIXLINEEXP,
        CELLEXP,
        SEQEXP,
        ARRAYLISTEXP,
        ASSIGNLISTEXP,
        VARDEC,
        FUNCTIONDEC,
        LISTEXP,
        OPTIMIZEDEXP
    };

    virtual ExpType getType() = 0;

    template <class T>
    inline T* getAs(void)
    {
        return static_cast<T*>(this);
    }

    inline virtual bool isSimpleVar() const
    {
        return false;
    }

    inline virtual bool isDollarVar() const
    {
        return false;
    }

    inline virtual bool isColonVar() const
    {
        return false;
    }

    inline virtual bool isArrayListVar() const
    {
        return false;
    }

    inline virtual bool isDoubleExp() const
    {
        return false;
    }

    inline virtual bool isBoolExp() const
    {
        return false;
    }

    inline virtual bool isStringExp() const
    {
        return false;
    }

    inline virtual bool isCommentExp() const
    {
        return false;
    }

    inline virtual bool isNilExp() const
    {
        return false;
    }

    inline virtual bool isCallExp() const
    {
        return false;
    }

    inline virtual bool isCellCallExp() const
    {
        return false;
    }

    inline virtual bool isOpExp() const
    {
        return false;
    }

    inline virtual bool isLogicalOpExp() const
    {
        return false;
    }

    inline virtual bool isAssignExp() const
    {
        return false;
    }

    inline virtual bool isIfExp() const
    {
        return false;
    }

    inline virtual bool isWhileExp() const
    {
        return false;
    }

    inline virtual bool isForExp() const
    {
        return false;
    }

    inline virtual bool isBreakExp() const
    {
        return false;
    }

    inline virtual bool isContinueExp() const
    {
        return false;
    }

    inline virtual bool isTryCatchExp() const
    {
        return false;
    }

    inline virtual bool isSelectExp() const
    {
        return false;
    }

    inline virtual bool isCaseExp() const
    {
        return false;
    }

    inline virtual bool isReturnExp() const
    {
        return false;
    }

    inline virtual bool isFieldExp() const
    {
        return false;
    }

    inline virtual bool isNotExp() const
    {
        return false;
    }

    inline virtual bool isTransposeExp() const
    {
        return false;
    }

    inline virtual bool isMatrixExp() const
    {
        return false;
    }

    inline virtual bool isMatrixLineExp() const
    {
        return false;
    }

    inline virtual bool isCellExp() const
    {
        return false;
    }

    inline virtual bool isSeqExp() const
    {
        return false;
    }

    inline virtual bool isArrayListExp() const
    {
        return false;
    }

    inline virtual bool isAssignListExp() const
    {
        return false;
    }

    inline virtual bool isVarDec() const
    {
        return false;
    }

    inline virtual bool isFunctionDec() const
    {
        return false;
    }

    inline virtual bool isListExp() const
    {
        return false;
    }

private:
    bool _verbose;
    bool _bBreak;
    bool _bBreakable;
    bool _bReturn;
    bool _bReturnable;
    bool _bContinue;
    bool _bContinuable;
};

/** \brief Define a shorthand for list of Exp* manipulation. */
typedef std::list<Exp *> exps_t;

} // namespace ast

#endif // !AST_EXP_HXX
