/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

// This code is separated in run_CallExp.hxx
// but will be inlined in runvisitor.hxx
// using #include with RunVisitorT class declaration.
//
// If you need additionnal headers, please add it in runvisitor.hxx

void visitprivate(const CallExp &e)
{
    T execFunc;
    std::list<Exp *>::const_iterator	itExp;

    e.name_get().accept(execFunc);
    if(execFunc.result_get() != NULL && execFunc.result_get()->isCallable())
    {//function call
        Callable *pCall = execFunc.result_get()->getAsCallable();
        types::typed_list out;
        types::typed_list in;

        //get function arguments
        T *execVar = new T[e.args_get().size()]();
        int j = 0;
        for (j = 0, itExp = e.args_get().begin (); itExp != e.args_get().end (); ++itExp,j++)
        {
            (*itExp)->accept (execVar[j]);
            if(execVar[j].result_get()->getType() == InternalType::RealImplicitList)
            {
                ImplicitList* pIL = execVar[j].result_get()->getAsImplicitList();
                execVar[j].result_set(pIL->extract_matrix());
                delete pIL;
            }
				
            if(execVar[j].is_single_result())
            {
                in.push_back(execVar[j].result_get());
                execVar[j].result_get()->IncreaseRef();
            }
            else
            {
                for(int i = 0 ; i < execVar[j].result_size_get() ; i++)
                {
                    in.push_back(execVar[j].result_get(i));
                    execVar[j].result_get(i)->IncreaseRef();
                }
            }
        }
			
        int iRetCount = Max(1, expected_size_get());

        T execCall;
        Function::ReturnValue Ret = pCall->call(in, iRetCount, out, &execCall);
			
        if(Ret == Callable::OK)
        {
            if(expected_size_get() == 1 && out.size() == 0) //to manage ans
            {
                if(static_cast<int>(out.size()) < expected_size_get())
                {
                    std::ostringstream os;
                    os << "bad lhs, expected : " << expected_size_get() << " returned : " << out.size() << std::endl;
                    throw os.str();
                }
            }

            if(out.size() == 1)
            {
                result_set(out[0]);
            }
            else 
            {
                for(int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                {
                    result_set(i, out[i]);
                }
            }
        }
        else if(Ret == Callable::Error)
        {
            std::ostringstream os;
            char szError[bsiz];
#ifdef _MSC_VER
            sprintf_s(szError, bsiz, _("Function \"%s\" failed\n"), pCall->getName().c_str());
#else
            sprintf(szError, _("Function \"%s\" failed\n"), pCall->getName().c_str());
#endif
            throw string(szError);
        }

			
        for (unsigned int k = 0; k < e.args_get().size(); k++)
        {
            execVar[k].result_get()->DecreaseRef();
        }
			
//			std::cout << "before delete[]" << std::endl;
        delete[] execVar;
//			std::cout << "after delete[]" << std::endl;
    }
    else if(execFunc.result_get() != NULL)
    {//a(xxx) with a variable, extraction

        //get symbol of variable
        InternalType *pIT = NULL;

        //WARNING can be a fieldexp
        const SimpleVar *Var = dynamic_cast<const SimpleVar*>(&e.name_get());
        if(Var != NULL)
        {
            pIT = symbol::Context::getInstance()->get(Var->name_get());
        }
        else
        {
            pIT = execFunc.result_get();
        }
        InternalType *pOut			= NULL;
        std::vector<InternalType*> ResultList;
        int iArgDim							= static_cast<int>(e.args_get().size());
        bool bSeeAsVector				= iArgDim == 1;

        //Create list of indexes
        //std::vector<std::vector<int>> IndexList;

        int *piIndexSeq		= NULL;
        int *piMaxDim			= NULL;
        int *piDimSize		= new int[iArgDim];
        int iTotalCombi		= GetIndexList(e.args_get(), &piIndexSeq, &piMaxDim, pIT, piDimSize);

        switch(pIT->getType())
        {
        case InternalType::RealDouble :
            pOut = pIT->getAsDouble()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
            break;
        case InternalType::RealBool : 
            pOut = pIT->getAsBool()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
            break;
        case InternalType::RealInt :
            pOut = pIT->getAsInt()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
            break;
        case InternalType::RealString :
            pOut = pIT->getAsString()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
            break;
        case InternalType::RealList :
        {
            ResultList = pIT->getAsList()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
            for(int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
            {
                result_set(i, ResultList[i]);
            }
            break;
        }
        case InternalType::RealCell :
            pOut = pIT->getAsCell()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
            break;
        default :
            break;
        }

        //List extraction can return multiple items
        if(pIT->isList() == false)
        {
            if(pOut == NULL)
            {
                std::ostringstream os;
                os << "inconsistent row/column dimensions";
                os << ((*e.args_get().begin())->location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pOut);
        }
        else 
        {
            if(ResultList.size() == 0)
            {
                std::ostringstream os;
                os << "inconsistent row/column dimensions";
                os << ((*e.args_get().begin())->location_get()).location_string_get() << std::endl;
                throw os.str();
            }
        }
			
        delete[] piDimSize;
        delete[] piIndexSeq;
        delete[] piMaxDim;
    }
    else
    {
        //result == NULL ,variable doesn't exist :(
        // Sould never be in this case
        // In worst case variable pointing to function does not exists
        // visitprivate(SimpleVar) will throw the right exception.
    }
}
