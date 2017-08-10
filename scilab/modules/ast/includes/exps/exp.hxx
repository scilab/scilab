/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
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

    const std::wstring getTypeString() const
    {
        switch (getType())
        {
            case SIMPLEVAR:
                return L"SimpleVar";
            case DOLLARVAR:
                return L"DollarVar";
            case COLONVAR:
                return L"ColonVar";
            case ARRAYLISTVAR:
                return L"ArrayListVar";
            case DOUBLEEXP:
                return L"DoubleExp";
            case BOOLEXP:
                return L"BoolExp";
            case STRINGEXP:
                return L"StringExp";
            case COMMENTEXP:
                return L"CommentExp";
            case CONSTEXP:
                return L"ConstExp";
            case NILEXP:
                return L"NilExp";
            case CALLEXP:
                return L"CallExp";
            case CELLCALLEXP:
                return L"CellCallExp";
            case OPEXP:
                return L"OpExp";
            case LOGICALOPEXP:
                return L"LogicalOpExp";
            case ASSIGNEXP:
                return L"AssignExp";
            case IFEXP:
                return L"IfExp";
            case WHILEEXP:
                return L"WhileExp";
            case FOREXP:
                return L"ForExp";
            case BREAKEXP:
                return L"BreakExp";
            case CONTINUEEXP:
                return L"ContinueExp";
            case TRYCATCHEXP:
                return L"TryCatchExp";
            case SELECTEXP:
                return L"SelectExp";
            case CASEEXP:
                return L"CaseExp";
            case RETURNEXP:
                return L"ReturnExp";
            case FIELDEXP:
                return L"FieldExp";
            case NOTEXP:
                return L"NotExp";
            case TRANSPOSEEXP:
                return L"TransposeExp";
            case MATRIXEXP:
                return L"MatrixExp";
            case MATRIXLINEEXP:
                return L"MatrixLineExp";
            case CELLEXP:
                return L"CellExp";
            case SEQEXP:
                return L"SeqExp";
            case ARRAYLISTEXP:
                return L"ArrayListExp";
            case ASSIGNLISTEXP:
                return L"AssignListExp";
            case VARDEC:
                return L"VarDec";
            case FUNCTIONDEC:
                return L"FunctionDec";
            case LISTEXP:
                return L"ListExp";
            case OPTIMIZEDEXP:
                return L"OptimizedExp";
            case MEMFILLEXP:
                return L"MemfillExp";
            case DAXPYEXP:
                return L"DAXPYExp";
            case STRINGSELECTEXP:
                return L"StringSelectExp";
            case TABLEINTSELECTEXP:
                return L"TableIntSelectExp";
            case MAPINTSELECTEXP:
                return L"MapIntSelectExp";
            case SMALLINTSELECTEXP:
                return L"SmallIntSelectExp";
            default:
                return L"????Exp";
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
