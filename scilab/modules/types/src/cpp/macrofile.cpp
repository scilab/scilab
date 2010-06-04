/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
* 
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
* 
*/

#include <sstream>
#include "macrofile.hxx"
#include "context.hxx"
#include "localization.h"
#include "yaspio.hxx"
#include "parser.hxx"
#include "configvariable.hxx"

using namespace ast;
namespace types
{

	/*--------------*/
	/*	Contructor  */
	/*--------------*/
	MacroFile::MacroFile(std::string _stName, string _stPath, string _stModule):
		Callable(),
		m_stPath(_stPath),
		m_pMacro(NULL)
	{
	  setName(_stName);
	  setModule(_stModule);
	}

	/*--------------*/
	/*	whoIAm		  */
	/*--------------*/
	void MacroFile::whoAmI()
	{
		std::cout << "types::MacroFile";
	}

	MacroFile* MacroFile::getAsMacroFile(void)
	{
		return this; 
	}

	InternalType::RealType MacroFile::getType(void)
	{ 
		return RealMacroFile; 
	}
	
	Macro* MacroFile::macro_get(void)
	{
		return m_pMacro;
	}

	std::string MacroFile::toString(int _iPrecision, int _iLineLen)
	{
	  std::ostringstream ostr;

	  // FIXME : Implement me.
	  ostr << "FIXME : Implement MacroFile::toString" << std::endl;

	  return ostr.str();
	}

	Callable::ReturnValue MacroFile::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
	{
		ReturnValue RetVal = Callable::OK;

		parse();
		if(m_pMacro)
		{
			ReturnValue Val =  m_pMacro->call(in, _iRetCount, out, execFunc);
			return Val;
		}
		else
		{
			return Callable::Error;
		}
	}

	bool MacroFile::parse(void)
	{

		if(m_pMacro == NULL)
		{//load file, only for the first call
			Parser parser;
            parser.parseFile(m_stPath, "parse macro file");
			//find FunctionDec
			FunctionDec* pFD = NULL;

			std::list<Exp *>::iterator j;
			std::list<Exp *>LExp = ((SeqExp*)parser.getTree())->exps_get();

			for(j = LExp.begin() ; j != LExp.end() ; j++)
			{
				pFD = dynamic_cast<FunctionDec*>(*j);
				if(pFD) // &&	pFD->name_get() == m_stName
				{
					symbol::Context* pContext = symbol::Context::getInstance();
					if(pContext->get_fun(pFD->name_get())->isMacroFile())
					{
						MacroFile* pMacro = pContext->get_fun(pFD->name_get())->getAsMacroFile();
						if(pMacro->m_pMacro == NULL)
						{
							std::list<Var *>::const_iterator	i;

							//get input parameters list
							std::list<std::string> *pVarList = new std::list<std::string>();
							ArrayListVar *pListVar = (ArrayListVar *)&pFD->args_get();
							for(i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
							{
								pVarList->push_back(((SimpleVar*)(*i))->name_get());
							}

							//get output parameters list
							std::list<std::string> *pRetList = new std::list<std::string>();
							ArrayListVar *pListRet = (ArrayListVar *)&pFD->returns_get();
							for(i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
							{
								pRetList->push_back(((SimpleVar*)(*i))->name_get());
							}

							//types::Macro macro(VarList, RetList, (SeqExp&)e.body_get());
							//types::Macro *pMacro = new types::Macro(m_stName, *pVarList, *pRetList, (SeqExp&)e.body_get());

							pMacro->m_pMacro = new Macro(m_stName, *pVarList, *pRetList, (SeqExp&)pFD->body_get(), m_stModule);
						}
					}
				}
			}
		}
		return true;
	}
}
