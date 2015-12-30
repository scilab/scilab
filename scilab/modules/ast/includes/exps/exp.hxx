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
#include <vector>
#include <algorithm>
#include "ast.hxx"

namespace ast
{

/** \brief Define a shorthand for list of Exp* manipulation. */
typedef std::vector<Exp *> exps_t;

/** \brief Abstract an Expression node. */
class Exp : public Ast
{
public:
    /** \name Ctor & dtor.
    ** \{ */

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
          _bContinuable(false),
          parent(NULL),
          original(NULL)
    {
        original = this;
    }
    /** \brief Destroys an Expression node. */
    virtual ~Exp ()
    {
        for (exps_t::const_iterator it = _exps.begin(); it != _exps.end(); ++it)
        {
            if (*it != NULL)
            {
                delete *it;
            }
        }

        if (original && original != this)
        {
            delete original;
        }
    }
    /** \} */

    virtual Exp* clone() = 0;

    virtual bool equal(const Exp & e) const
    {
        if (getType() == e.getType() && _exps.size() == e._exps.size())
        {
            for (exps_t::const_iterator i = _exps.begin(), j = e._exps.begin(), _e = _exps.end(); i != _e; ++i, ++j)
            {
                if (!(*i)->equal(**j))
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    inline bool operator==(const Exp & R) const
    {
        return equal(R);
    }

    inline bool operator!=(const Exp & R) const
    {
        return !equal(R);
    }

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

    //manage break instruction
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

    virtual inline void setBreakable(void)
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

    //manage return instruction
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

    virtual inline void setReturnable(void)
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

    //manage continue instruction
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

    virtual inline void setContinuable(void)
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
        CONSTEXP,
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
        OPTIMIZEDEXP,
        MEMFILLEXP,
        DAXPYEXP,
        STRINGSELECTEXP,
        TABLEINTSELECTEXP,
        MAPINTSELECTEXP,
        SMALLINTSELECTEXP,
    };

    const std::string getTypeString() const
    {
        switch (getType())
        {
            case SIMPLEVAR:
                return "SimpleVar";
            case DOLLARVAR:
                return "DollarVar";
            case COLONVAR:
                return "ColonVar";
            case ARRAYLISTVAR:
                return "ArrayListVar";
            case DOUBLEEXP:
                return "DoubleExp";
            case BOOLEXP:
                return "BoolExp";
            case STRINGEXP:
                return "StringExp";
            case COMMENTEXP:
                return "CommentExp";
            case CONSTEXP:
                return "ConstExp";
            case NILEXP:
                return "NilExp";
            case CALLEXP:
                return "CallExp";
            case CELLCALLEXP:
                return "CellCallExp";
            case OPEXP:
                return "OpExp";
            case LOGICALOPEXP:
                return "LogicalOpExp";
            case ASSIGNEXP:
                return "AssignExp";
            case IFEXP:
                return "IfExp";
            case WHILEEXP:
                return "WhileExp";
            case FOREXP:
                return "ForExp";
            case BREAKEXP:
                return "BreakExp";
            case CONTINUEEXP:
                return "ContinueExp";
            case TRYCATCHEXP:
                return "TryCatchExp";
            case SELECTEXP:
                return "SelectExp";
            case CASEEXP:
                return "CaseExp";
            case RETURNEXP:
                return "ReturnExp";
            case FIELDEXP:
                return "FieldExp";
            case NOTEXP:
                return "NotExp";
            case TRANSPOSEEXP:
                return "TransposeExp";
            case MATRIXEXP:
                return "MatrixExp";
            case MATRIXLINEEXP:
                return "MatrixLineExp";
            case CELLEXP:
                return "CellExp";
            case SEQEXP:
                return "SeqExp";
            case ARRAYLISTEXP:
                return "ArrayListExp";
            case ASSIGNLISTEXP:
                return "AssignListExp";
            case VARDEC:
                return "VarDec";
            case FUNCTIONDEC:
                return "FunctionDec";
            case LISTEXP:
                return "ListExp";
            case OPTIMIZEDEXP:
                return "OptimizedExp";
            case MEMFILLEXP:
                return "MemfillExp";
            case DAXPYEXP:
                return "DAXPYExp";
            case STRINGSELECTEXP:
                return "StringSelectExp";
            case TABLEINTSELECTEXP:
                return "TableIntSelectExp";
            case MAPINTSELECTEXP:
                return "MapIntSelectExp";
            case SMALLINTSELECTEXP:
                return "SmallIntSelectExp";
            default:
                return "????Exp";
        }
    }

    virtual ExpType getType() const = 0;

    template <class T>
    inline T* getAs(void)
    {
        return static_cast<T*>(this);
    }

    template <class T>
    inline const T* getAs(void) const
    {
        return static_cast<const T*>(this);
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

    inline virtual bool isConstExp() const
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

    inline virtual bool isControlExp() const
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

    inline virtual bool isOptimizedExp() const
    {
        return false;
    }

    inline virtual bool isMemfillExp() const
    {
        return false;
    }

    inline virtual bool isFastSelectExp() const
    {
        return false;
    }

    inline virtual bool isStringSelectExp() const
    {
        return false;
    }

    inline virtual bool isIntSelectExp() const
    {
        return false;
    }

    inline virtual bool isTableIntSelectExp() const
    {
        return false;
    }

    inline virtual bool isMapIntSelectExp() const
    {
        return false;
    }

    inline virtual bool isSmallIntSelectExp() const
    {
        return false;
    }

    inline Exp* getParent() const
    {
        return parent;
    }

    inline Exp* getParent()
    {
        return parent;
    }

    inline void setParent(Exp* _ast)
    {
        parent = _ast;
    }

    inline Exp* getOriginal() const
    {
        return original;
    }

    inline Exp* getOriginal()
    {
        return original;
    }

    inline void setOriginal(Exp* _ast)
    {
        original = _ast;
    }

    inline void replace(Exp* _new)
    {
        if (parent && _new)
        {
            parent->replace(this, _new);
        }
    }

    inline void replace(Exp* _old, Exp* _new)
    {
        if (_old && _new)
        {
            for (exps_t::iterator it = _exps.begin(), itEnd = _exps.end(); it != itEnd; ++it)
            {
                if (*it == _old)
                {
                    _new->setOriginal(_old);
                    *it = _new;
                    _new->setParent(this);
                    return;
                }
            }
        }
    }

    inline const exps_t & getExps() const
    {
        return _exps;
    }

    inline exps_t & getExps()
    {
        return _exps;
    }


private:
    bool _verbose;
    bool _bBreak;
    bool _bBreakable;
    bool _bReturn;
    bool _bReturnable;
    bool _bContinue;
    bool _bContinuable;
protected:
    exps_t _exps;
    Exp* parent;
    Exp* original;
};
} // namespace ast

#endif // !AST_EXP_HXX
