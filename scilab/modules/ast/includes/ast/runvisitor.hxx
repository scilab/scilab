/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_RUNVISITOR_HXX
#define AST_RUNVISITOR_HXX

#include <set>

#include "all.hxx"
#include "types.hxx"
#include "alltypes.hxx"
#include "configvariable.hxx"
#include "overload.hxx"
#include "scilabWrite.hxx"
#include "context.hxx"
#include "variables.hxx"

extern "C" {
#include "more.h"
#include "sci_malloc.h"
}

using namespace types;

namespace ast
{
class EXTERN_AST RunVisitor : public ConstVisitor
{
public:
    RunVisitor()
    {
        _excepted_result = -1;
        _resultVect.push_back(NULL);
        _result = NULL;
        m_bSingleResult = true;
        m_pAns = symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"ans"));
    }

    ~RunVisitor()
    {
        clearResult();
    }

    void clearResultButFirst()
    {
        if (!isSingleResult() && _resultVect.size() > 1)
        {
            for (vector<types::InternalType*>::iterator rv = _resultVect.begin() + 1, end = _resultVect.end(); rv != end; ++rv)
            {
                if (*rv != NULL)
                {
                    (*rv)->killMe();
                    *rv = NULL;
                }
            }
        }
    }

    void clearResult()
    {
        if (isSingleResult())
        {
            if (_result != NULL)
            {
                //                    std::cout << "before single delete : " << _result << std::endl;
                _result->killMe();
                //                    std::cout << "after single delete" << std::endl;
            }
        }
        else
        {
            for (vector<types::InternalType*>::iterator rv = _resultVect.begin(), end = _resultVect.end(); rv != end; ++rv)
            {
                if (*rv != NULL)
                {
                    (*rv)->killMe();
                }
            }
        }
        _resultVect.clear();
        m_bSingleResult = true;
        _result = NULL;
    }

public:
    int getExpectedSize(void)
    {
        return _excepted_result;
    }

    void setExpectedSize(int _iSize)
    {
        _excepted_result = _iSize;
    }

    int getResultSize(void)
    {
        if (isSingleResult())
        {
            if (_result == NULL)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return static_cast<int>(_resultVect.size());
        }
    }

    inline types::InternalType* getResult(void)
    {
        if (isSingleResult())
        {
            return _result;
        }
        else
        {
            return _resultVect[0];
        }
    }

    types::InternalType* getResult(int _iPos)
    {
        if (isSingleResult() && _iPos == 0)
        {
            return _result;
        }

        if (_iPos >= static_cast<int>(_resultVect.size()))
        {
            return NULL;
        }
        return _resultVect[_iPos];
    }

    vector<types::InternalType*>* getResultList()
    {
        // TODO: this function is not used but it could lead to a memleak
        // (in the first case the vector is allocated and so must be freed)
        if (getResultSize() == 1)
        {
            vector<types::InternalType*>* pList = new vector<types::InternalType*>;
            pList->push_back(_result);
            return pList;
        }
        else
        {
            return &_resultVect;
        }
    }

    void setResult(int _iPos, const types::InternalType *gtVal)
    {
        m_bSingleResult = false;
        if (_iPos < static_cast<int>(_resultVect.size()))
        {
            if (_resultVect[_iPos])
            {
                _resultVect[_iPos]->killMe();
            }
        }
        else
        {
            _resultVect.resize(_iPos + 1, NULL);
        }

        _resultVect[_iPos] = const_cast<types::InternalType *>(gtVal);
    }

    inline void setResult(const types::InternalType *gtVal)
    {
        m_bSingleResult = true;
        _result = const_cast<types::InternalType *>(gtVal);
    }

    inline void setResult(const types::typed_list & out)
    {
        if (out.size() == 0)
        {
            setResult(NULL);
        }
        else if (out.size() == 1)
        {
            setResult(out[0]);
        }
        else
        {
            /*for (int i = 0 ; i < static_cast<int>(out.size()) ; i++)
            {
            setResult(i, out[i]);
            }*/

            m_bSingleResult = false;
            for (vector<types::InternalType*>::const_iterator it = _resultVect.begin(); it != _resultVect.end(); ++it)
            {
                if (*it)
                {
                    (*it)->killMe();
                }
            }
            _resultVect.clear();

            for (types::typed_list::const_iterator it = out.begin(); it != out.end(); ++it)
            {
                _resultVect.push_back(*it);
            }
        }
    }

    inline bool isSingleResult()
    {
        return m_bSingleResult;
    }

    void cleanIn(const types::typed_list & in, const types::typed_list & out)
    {
        // Check if in contains entries which are in out too.
        // When an entry is in in and not in out, then in is killed.
        if (!in.empty())
        {
            for (types::typed_list::const_iterator o = out.begin(); o != out.end(); ++o)
            {
                if (*o)
                {
                    (*o)->IncreaseRef();
                }
            }

            for (types::typed_list::const_iterator i = in.begin(); i != in.end(); ++i)
            {
                if (*i)
                {
                    (*i)->DecreaseRef();
                    (*i)->killMe();
                }
            }

            for (types::typed_list::const_iterator o = out.begin(); o != out.end(); ++o)
            {
                if (*o)
                {
                    (*o)->DecreaseRef();
                }
            }
        }
    }

    inline void cleanInOut(const types::typed_list & in, const types::typed_list & out)
    {
        cleanIn(in, out);
        cleanOut(out);
    }

    void cleanOut(const types::typed_list & out)
    {
        if (!out.empty())
        {
            for (types::typed_list::const_iterator o = out.begin(); o != out.end(); ++o)
            {
                if (*o)
                {
                    (*o)->killMe();
                }
            }
        }
    }

    void cleanOpt(const types::optional_list & opt)
    {
        if (!opt.empty())
        {
            for (types::optional_list::const_iterator o = opt.begin(); o != opt.end(); ++o)
            {
                if (o->second)
                {
                    //decreasef ref after increaseref in callexp
                    o->second->DecreaseRef();
                    o->second->killMe();
                }
            }
        }
    }

    /*-------------.
    | Attributes.  |
    `-------------*/
protected:
    vector<types::InternalType*>    _resultVect;
    types::InternalType*    _result;
    bool m_bSingleResult;
    int _excepted_result;
    symbol::Variable* m_pAns;
};

template <class T>
class EXTERN_AST RunVisitorT : public RunVisitor
{
public :
    RunVisitorT() : RunVisitor()
    {
    }

    types::typed_list* GetArgumentList(std::list<Exp *>const& _plstArg)
    {
        types::typed_list* pArgs = new types::typed_list();
        for (std::list<Exp *>::const_iterator it = _plstArg.begin() ; it != _plstArg.end() ; ++it)
        {
            (*it)->accept(*this);
            if (getResultSize() > 1)
            {
                const int size = getResultSize();
                for (int i = 0 ; i < size; i++)
                {
                    pArgs->push_back(getResult(i));
                }
            }
            else
            {
                pArgs->push_back(getResult());
            }
        }
        //to be sure, delete operation does not delete result
        setResult(NULL);
        return pArgs;
    }

public :
    //process in another node
    void visitprivate(const MatrixLineExp &/*e*/) {}
    void visitprivate(const CommentExp &/*e*/) {}
    void visitprivate(const ArrayListVar &/*e*/) {}
    void visitprivate(const CaseExp &/*e*/) {}
    void visitprivate(const AssignListExp  &/*e*/) {}

    void visitprivate(const CellExp &e);
    void visitprivate(const FieldExp &e);
    void visitprivate(const IfExp  &e);
    void visitprivate(const WhileExp  &e);
    void visitprivate(const ForExp  &e);
    void visitprivate(const ReturnExp &e);
    void visitprivate(const SelectExp &e);
    void visitprivate(const SeqExp  &e);
    void visitprivate(const NotExp &e);
    void visitprivate(const TransposeExp &e);
    void visitprivate(const FunctionDec  &e);
    void visitprivate(const ListExp &e);
    void visitprivate(const AssignExp  &e);
    void visitprivate(const OpExp &e);
    void visitprivate(const LogicalOpExp &e);
    void visitprivate(const MatrixExp &e);
    void visitprivate(const CallExp &e);
    void visitprivate(const CellCallExp &e);

    void visitprivate(const StringExp &e)
    {
        if (e.getBigString() == NULL)
        {
            types::String *psz = new types::String(e.getValue().c_str());
            (const_cast<StringExp *>(&e))->setBigString(psz);

        }
        setResult(e.getBigString());
    }


    void visitprivate(const DoubleExp  &e)
    {
        if (e.getBigDouble() == NULL)
        {
            Double *pdbl = new Double(e.getValue());
            (const_cast<DoubleExp *>(&e))->setBigDouble(pdbl);

        }
        setResult(e.getBigDouble());
    }


    void visitprivate(const BoolExp  &e)
    {
        if (e.getBigBool() == NULL)
        {
            Bool *pB = new Bool(e.getValue());
            (const_cast<BoolExp *>(&e))->setBigBool(pB);

        }
        setResult(e.getBigBool());
    }


    void visitprivate(const NilExp &/*e*/)
    {
        setResult(new types::Void());
    }


    void visitprivate(const SimpleVar &e)
    {
        InternalType *pI = symbol::Context::getInstance()->get(((SimpleVar&)e).getStack());
        setResult(pI);
        if (pI != NULL)
        {
            if (e.isVerbose() && pI->isCallable() == false && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << e.getSymbol().getName() << L"  = " << L"(" << pI->getRef() << L")" << std::endl;
                ostr << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pI, e.getSymbol().getName().c_str());
            }
        }
        else
        {
            char pstError[bsiz];
            wchar_t* pwstError;

            char* strErr =  wide_string_to_UTF8(e.getSymbol().getName().c_str());

            sprintf(pstError, _("Undefined variable: %s\n"), strErr);
            pwstError = to_wide_string(pstError);
            FREE(strErr);
            std::wstring wstError(pwstError);
            FREE(pwstError);
            throw ScilabError(wstError, 999, e.getLocation());
            //Err, SimpleVar doesn't exist in Scilab scopes.
        }
    }


    void visitprivate(const ColonVar &/*e*/)
    {
        Colon *pC = new Colon();
        setResult(pC);
    }


    void visitprivate(const DollarVar &/*e*/)
    {
        Dollar* pVar = new Dollar();
        setResult(pVar);
    }

    void visitprivate(const TryCatchExp  &e)
    {
        //save current prompt mode
        int oldVal = ConfigVariable::getSilentError();
        //set mode silent for errors
        ConfigVariable::setSilentError(1);
        try
        {
            e.getTry().accept(*this);
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
        }
        catch (ScilabMessage sm)
        {
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
            //to lock lasterror
            ConfigVariable::setLastErrorCall();
            e.getCatch().accept(*this);
        }
    }

    void visitprivate(const BreakExp &e)
    {
        const_cast<BreakExp*>(&e)->setBreak();
    }

    void visitprivate(const ContinueExp &e)
    {
        const_cast<ContinueExp*>(&e)->setContinue();
    }

    void visitprivate(const ArrayListExp  &e)
    {
        std::list<Exp *>::const_iterator it;
        int i = 0;

        std::list<InternalType*> lstIT;
        for (it = e.getExps().begin() ; it != e.getExps().end() ; it++)
        {
            (*it)->accept(*this);
            lstIT.push_back(getResult()->clone());
        }

        std::list<InternalType*>::iterator itIT = lstIT.begin();
        for (; itIT != lstIT.end(); itIT++)
        {
            setResult(i++, *itIT);
        }
    }

    void visitprivate(const VarDec  &e)
    {
        try
        {
            /*getting what to assign*/
            e.getInit().accept(*this);
            getResult()->IncreaseRef();
        }
        catch (ScilabError error)
        {
            throw error;
        }
    }

    void VariableToString(types::InternalType* pIT, const wchar_t* wcsVarName)
    {
        std::wostringstream ostr;

        if (pIT->isMList() || pIT->isTList() || pIT->hasToString() == false)
        {
            //call overload %type_p
            types::typed_list in;
            types::typed_list out;

            pIT->IncreaseRef();
            in.push_back(pIT);

            try
            {
                if (Overload::generateNameAndCall(L"p", in, 1, out, this) != Function::OK)
                    //if (Overload::call(L"%" + pIT->getAs<TList>()->getShortTypeStr() + L"_p", in, 1, out, this) != Function::OK)
                {
                    throw ScilabError();
                }
            }
            catch (ScilabError /*&e*/)
            {
                ostr << wcsVarName;
                pIT->toString(ostr);
                scilabWriteW(ostr.str().c_str());
            }

            pIT->DecreaseRef();
        }
        else
        {
            //to manage lines information
            int iLines = ConfigVariable::getConsoleLines();

            bool bFinish = false;
            do
            {
                //block by block
                bFinish = pIT->toString(ostr);
                scilabWriteW(ostr.str().c_str());
                if (bFinish == false && iLines != 0)
                {
                    //show message on prompt
                    bFinish = linesmore() == 1;
                }
                ostr.str(L"");
            }
            while (bFinish == false);

            pIT->clearPrintState();
        }
    }

    types::InternalType* callOverloadOpExp(OpExp::Oper _oper, types::InternalType* _paramL, types::InternalType* _paramR);
    types::InternalType* callOverloadMatrixExp(std::wstring strType, types::InternalType* _paramL, types::InternalType* _paramR);
};
}

#endif // !AST_RUNVISITOR_HXX
