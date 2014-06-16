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
class RunVisitor : public ConstVisitor
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
        result_clear();
    }

    void result_clear()
    {
        if (is_single_result())
        {
            if (_result != NULL && _result->isDeletable() == true)
            {
                //					std::cout << "before single delete : " << _result << std::endl;
                delete _result;
                //					std::cout << "after single delete" << std::endl;
            }
            _result = NULL;
        }
        else
        {
            for (unsigned int i = 0 ; i < _resultVect.size() ; i++)
            {
                if (_resultVect[i] != NULL && _resultVect[i]->isDeletable() == true)
                {
                    delete _resultVect[i];
                }
                _resultVect[i] = NULL;
            }
        }
        _resultVect.clear();
        m_bSingleResult = true;
        _result = NULL;
    }

public:
    int expected_getSize(void)
    {
        return _excepted_result;
    }

    int result_getSize(void)
    {
        if (is_single_result())
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

    void expected_setSize(int _iSize)
    {
        _excepted_result = _iSize;
    }

    types::InternalType* result_get(void)
    {
        if (is_single_result())
        {
            return _result;
        }
        else
        {
            return _resultVect[0];
        }
    }

    types::InternalType* result_get(int _iPos)
    {
        if (is_single_result() && _iPos == 0)
        {
            return _result;
        }

        if (_iPos >= static_cast<int>(_resultVect.size()))
        {
            return NULL;
        }
        return _resultVect[_iPos];
    }

    vector<types::InternalType*>* result_list_get()
    {
        if (result_getSize() == 1)
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

    void result_set(int _iPos, const types::InternalType *gtVal)
    {
        m_bSingleResult = false;
        if (_iPos <  static_cast<int>(_resultVect.size()))
        {
            if (_resultVect[_iPos] != NULL && _resultVect[_iPos]->isDeletable())
            {
                delete _resultVect[_iPos];
            }
        }

        if (_iPos >=  static_cast<int>(_resultVect.size()))
        {
            _resultVect.resize(_iPos + 1, NULL);
        }

        _resultVect[_iPos] = const_cast<types::InternalType *>(gtVal);
    }

    void result_set(const types::InternalType *gtVal)
    {
        m_bSingleResult = true;
        _result = const_cast<types::InternalType *>(gtVal);
    }

    bool is_single_result()
    {
        return m_bSingleResult;
    }

    /*-------------.
    | Attributes.  |
    `-------------*/
protected:
    vector<types::InternalType*>	_resultVect;
    types::InternalType*	_result;
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
        std::list<Exp *>::const_iterator it;
        for (it = _plstArg.begin() ; it != _plstArg.end() ; it++)
        {
            (*it)->accept(*this);
            if (result_getSize() > 1)
            {
                for (int i = 0 ; i < result_getSize() ; i++)
                {
                    pArgs->push_back(result_get(i));
                }
            }
            else
            {
                pArgs->push_back(result_get());
            }
        }
        //to be sure, delete operation does not delete result
        result_set(NULL);
        return pArgs;
    }

public :
    //not use
    void visitprivate(const IntExp  &/*e*/) {}
    void visitprivate(const FloatExp  &/*e*/) {}

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
            types::String *psz = new types::String(e.value_get().c_str());
            (const_cast<StringExp *>(&e))->setBigString(psz);

        }
        result_set(e.getBigString());
    }


    void visitprivate(const DoubleExp  &e)
    {
        if (e.getBigDouble() == NULL)
        {
            Double *pdbl = new Double(e.value_get());
            (const_cast<DoubleExp *>(&e))->setBigDouble(pdbl);

        }
        result_set(e.getBigDouble());
    }


    void visitprivate(const BoolExp  &e)
    {
        if (e.getBigBool() == NULL)
        {
            Bool *pB = new Bool(e.value_get());
            (const_cast<BoolExp *>(&e))->setBigBool(pB);

        }
        result_set(e.getBigBool());
    }


    void visitprivate(const NilExp &e)
    {
        result_set(new types::Void());
    }


    void visitprivate(const SimpleVar &e)
    {
        InternalType *pI = symbol::Context::getInstance()->get(((SimpleVar&)e).stack_get());
        result_set(pI);
        if (pI != NULL)
        {
            if (e.is_verbose() && pI->isCallable() == false && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << e.name_get().name_get() << L"  = " << L"(" << pI->getRef() << L")" << std::endl;
                ostr << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pI, e.name_get().name_get().c_str());
            }
        }
        else
        {
            char pstError[bsiz];
            wchar_t* pwstError;

            char* strErr =  wide_string_to_UTF8(e.name_get().name_get().c_str());

            sprintf(pstError, _("Undefined variable: %s\n"), strErr);
            pwstError = to_wide_string(pstError);
            FREE(strErr);
            std::wstring wstError(pwstError);
            FREE(pwstError);
            throw ScilabError(wstError, 999, e.location_get());
            //Err, SimpleVar doesn't exist in Scilab scopes.
        }
    }


    void visitprivate(const ColonVar &e)
    {
        Colon *pC = new Colon();
        result_set(pC);
    }


    void visitprivate(const DollarVar &e)
    {
        Dollar* pVar = new Dollar();
        result_set(pVar);
    }

    void visitprivate(const TryCatchExp  &e)
    {
        //save current prompt mode
        int oldVal = ConfigVariable::getSilentError();
        //set mode silent for errors
        ConfigVariable::setSilentError(1);
        try
        {
            e.try_get().accept(*this);
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
        }
        catch (ScilabMessage sm)
        {
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
            //to lock lasterror
            ConfigVariable::setLastErrorCall();
            e.catch_get().accept(*this);
        }
    }

    void visitprivate(const BreakExp &e)
    {
        const_cast<BreakExp*>(&e)->break_set();
    }

    void visitprivate(const ContinueExp &e)
    {
        const_cast<ContinueExp*>(&e)->continue_set();
    }

    void visitprivate(const ArrayListExp  &e)
    {
        std::list<Exp *>::const_iterator it;
        int i = 0;

        std::list<InternalType*> lstIT;
        for (it = e.exps_get().begin() ; it != e.exps_get().end() ; it++)
        {
            (*it)->accept(*this);
            lstIT.push_back(result_get()->clone());
        }

        std::list<InternalType*>::iterator itIT = lstIT.begin();
        for (; itIT != lstIT.end(); itIT++)
        {
            result_set(i++, *itIT);
        }
    }

    void visitprivate(const VarDec  &e)
    {
        try
        {
            /*getting what to assign*/
            e.init_get().accept(*this);
            result_get()->IncreaseRef();
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
                if (Overload::call(L"%" + pIT->getAs<TList>()->getShortTypeStr() + L"_p", in, 1, out, this) != Function::OK)
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
