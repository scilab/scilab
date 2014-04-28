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

#include <fstream>
#include <iostream>
#include <sstream>
#include "macrofile.hxx"
#include "context.hxx"
#include "localization.h"
#include "scilabWrite.hxx"
#include "parser.hxx"
#include "configvariable.hxx"
#include "deserializervisitor.hxx"

using namespace ast;
namespace types
{
MacroFile::MacroFile(wstring _stName, wstring _stPath, wstring _stModule) : Callable(), m_stPath(_stPath), m_pMacro(NULL)
{
    setName(_stName);
    setModule(_stModule);
}

MacroFile::~MacroFile()
{
    if (m_pMacro)
    {
        delete m_pMacro;
    }
}

InternalType* MacroFile::clone()
{
    IncreaseRef();
    return this;
}

void MacroFile::whoAmI()
{
    std::cout << "types::MacroFile";
}

InternalType::ScilabType MacroFile::getType(void)
{
    return ScilabMacroFile;
}

bool MacroFile::toString(std::wostringstream& ostr)
{
    ostr << L"FIXME : Implement MacroFile::toString" << std::endl;
    return true;
}

Callable::ReturnValue MacroFile::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
{
    ReturnValue RetVal = Callable::OK;

    parse();
    if (m_pMacro)
    {
        ReturnValue Val =  m_pMacro->call(in, opt, _iRetCount, out, execFunc);
        return Val;
    }
    else
    {
        return Callable::Error;
    }
}

bool MacroFile::parse(void)
{

    if (m_pMacro == NULL)
    {
        //load file, only for the first call
        std::ifstream f(wide_string_to_UTF8(m_stPath.c_str()), ios::in | ios::binary | ios::ate);

        int size = (int)f.tellg();
        unsigned char* binAst = new unsigned char[size];
        f.seekg(0);
        f.read((char*)binAst, size);
        f.close();
        ast::DeserializeVisitor* d = new ast::DeserializeVisitor(binAst);
        ast::Exp* tree = d->deserialize();

        //find FunctionDec
        FunctionDec* pFD = NULL;

        std::list<ast::Exp *>::iterator j;
        std::list<ast::Exp *>LExp = ((ast::SeqExp*)tree)->exps_get();

        for (j = LExp.begin() ; j != LExp.end() ; j++)
        {
            pFD = dynamic_cast<FunctionDec*>(*j);
            if (pFD) // &&	pFD->name_get() == m_stName
            {
                symbol::Context* pContext = symbol::Context::getInstance();
                types::InternalType* pFunc = pContext->getFunction(pFD->name_get());
                if (pFunc && pFunc->isMacroFile())
                {
                    MacroFile* pMacro = pContext->getFunction(pFD->name_get())->getAs<MacroFile>();
                    if (pMacro->m_pMacro == NULL)
                    {
                        std::list<Var *>::const_iterator	i;

                        //get input parameters list
                        std::list<symbol::Symbol> *pVarList = new std::list<symbol::Symbol>();
                        ArrayListVar *pListVar = (ArrayListVar *)&pFD->args_get();
                        for (i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
                        {
                            pVarList->push_back(((SimpleVar*)(*i))->name_get());
                        }

                        //get output parameters list
                        std::list<symbol::Symbol> *pRetList = new std::list<symbol::Symbol>();
                        ArrayListVar *pListRet = (ArrayListVar *)&pFD->returns_get();
                        for (i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
                        {
                            pRetList->push_back(((SimpleVar*)(*i))->name_get());
                        }

                        //types::Macro macro(VarList, RetList, (SeqExp&)e.body_get());
                        //types::Macro *pMacro = new types::Macro(m_stName, *pVarList, *pRetList, (SeqExp&)e.body_get());

                        pMacro->m_pMacro = new Macro(m_wstName, *pVarList, *pRetList, (SeqExp&)pFD->body_get(), m_wstModule);
                        pMacro->setFirstLine(pFD->location_get().first_line);
                    }
                }
            }
        }
    }
    return true;
}

int MacroFile::getNbInputArgument(void)
{
    return getMacro()->getNbInputArgument();
}

int MacroFile::getNbOutputArgument(void)
{
    return getMacro()->getNbOutputArgument();
}

Macro* MacroFile::getMacro(void)
{
    parse();
    return m_pMacro;
}

void MacroFile::setFirstLine(int _iLine)
{
    getMacro()->setFirstLine(_iLine);
}
}
