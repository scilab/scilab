/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "debuggervisitor.hxx"
#include "debugmanager.hxx"
#include "printvisitor.hxx"
#include "execvisitor.hxx"
#include "threadId.hxx"

extern "C"
{
#include "filemanager_interface.h"
}

namespace ast
{
void DebuggerVisitor::visit(const SeqExp  &e)
{
    RunVisitor* exec = NULL;
    std::list<Exp *>::const_iterator itExp;
    debugger::DebuggerMagager* manager = debugger::DebuggerMagager::getInstance();

    if (ConfigVariable::getEnableDebug() == false)
    {
        //enable debugger for next execution
        ConfigVariable::setEnableDebug(true);

        ExecVisitor exec;
        e.accept(exec);
        return;
    }

    for (const auto& exp : e.getExps())
    {
        if (e.isBreakable())
        {
            exp->resetBreak();
            exp->setBreakable();
        }

        if (e.isContinuable())
        {
            exp->resetContinue();
            exp->setContinuable();
        }

        if (e.isReturnable())
        {
            exp->setReturnable();
        }

        //debugger check !
        if (ConfigVariable::getEnableDebug())
        {
            std::vector<ConfigVariable::WhereEntry> lWhereAmI = ConfigVariable::getWhere();
            int iLine = (exp->getLocation().first_line - ConfigVariable::getMacroFirstLines()) + 1;

            //manage step next
            if (manager->isStepNext())
            {
                manager->resetStepNext();
                manager->stop(exp, -1);
            }
            else if (manager->isStepIn())
            {
                manager->resetStepIn();
                manager->stop(exp, -1);
            }
            else if (manager->isStepOut())
            {
                manager->resetStepOut();
                manager->stop(exp, -1);
            }
            else
            {
                //set information from debugger commands
                if (lWhereAmI.size() != 0 && manager->getBreakPointCount() != 0)
                {
                    debugger::Breakpoints bps = manager->getAllBreakPoint();
                    std::wstring functionName = lWhereAmI.back().m_name;
                    int i = -1;
                    for (const auto& bp : bps)
                    {
                        ++i;
                        if (bp->isEnable() == false)
                        {
                            continue;
                        }

                        if (functionName == bp->getFunctioName())
                        {
                            if (bp->getMacroLine() == -1)
                            {
                                //first pass in macro.
                                //update first line with real value
                                bp->setMacroLine(iLine);
                            }

                            if (bp->getMacroLine() == iLine)
                            {
                                //check condition
                                if (bp->getConditionExp() != NULL)
                                {
                                    //do not use debuggervisitor !
                                    symbol::Context* pCtx = symbol::Context::getInstance();
                                    try
                                    {
                                        ExecVisitor execCond;
                                        //protect current env during condition execution
                                        pCtx->scope_begin();
                                        bp->getConditionExp()->accept(execCond);
                                        types::InternalType* pIT = pCtx->getCurrentLevel(symbol::Symbol(L"ans"));
                                        if (pIT == NULL ||
                                                pIT->isBool() == false ||
                                                ((types::Bool*)pIT)->isScalar() == false ||
                                                ((types::Bool*)pIT)->get(0) == 0)
                                        {
                                            pCtx->scope_end();
                                            //not a boolean, not scalar or false
                                            continue;
                                        }

                                        pCtx->scope_end();
                                        //ok condition is valid and true
                                    }
                                    catch (ast::ScilabException &/*e*/)
                                    {
                                        pCtx->scope_end();
                                        //not work !
                                        //invalid breakpoint
                                        continue;
                                    }
                                }

                                //we have a breakpoint !
                                //stop execution and wait signal from debugger to restart
                                manager->stop(exp, i);

                                //only one breakpoint can be "call" on same exp
                                break;
                            }
                        }
                    }
                }
            }
            exec = this;
        }
        else
        {
            //change visitor to execvitor instead of debuggervisitor
            exec = new ExecVisitor();
        }

        //copy from runvisitor::seqexp
        try
        {
            //reset default values
            setResult(NULL);
            int iExpectedSize = getExpectedSize();
            setExpectedSize(-1);
            exp->accept(*exec);
            setExpectedSize(iExpectedSize);
            types::InternalType * pIT = getResult();

            // In case of exec file, set the file name in the Macro to store where it is defined.
            int iFileID = ConfigVariable::getExecutedFileID();
            if (iFileID && exp->isFunctionDec())
            {
                types::InternalType* pITMacro = symbol::Context::getInstance()->get(exp->getAs<ast::FunctionDec>()->getSymbol());
                if (pITMacro)
                {
                    types::Macro* pMacro = pITMacro->getAs<types::Macro>();
                    const wchar_t* filename = getfile_filename(iFileID);
                    // scilab.quit is not open with mopen
                    // in this case filename is NULL because FileManager have not been filled.
                    if (filename)
                    {
                        pMacro->setFileName(filename);
                    }
                }
            }

            if (pIT != NULL)
            {
                bool bImplicitCall = false;
                if (pIT->isCallable()) //to manage call without ()
                {
                    types::Callable *pCall = pIT->getAs<types::Callable>();
                    types::typed_list out;
                    types::typed_list in;
                    types::optional_list opt;

                    try
                    {
                        //in this case of calling, we can return only one values
                        int iSaveExpectedSize = getExpectedSize();
                        setExpectedSize(1);

                        pCall->invoke(in, opt, getExpectedSize(), out, e);
                        setExpectedSize(iSaveExpectedSize);

                        if (out.size() == 0)
                        {
                            setResult(NULL);
                        }
                        else
                        {
                            setResult(out[0]);
                        }

                        bImplicitCall = true;
                    }
                    catch (const InternalError& ie)
                    {
                        if (ConfigVariable::getLastErrorFunction() == L"")
                        {
                            ConfigVariable::setLastErrorFunction(pCall->getName());
                            ConfigVariable::setLastErrorLine(e.getLocation().first_line);
                        }

                        throw ie;
                    }
                }

                //don't output Simplevar and empty result
                if (getResult() != NULL && (!exp->isSimpleVar() || bImplicitCall))
                {
                    //symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *execMe.getResult());
                    types::InternalType* pITAns = getResult();
                    symbol::Context::getInstance()->put(m_pAns, pITAns);
                    if (exp->isVerbose() && ConfigVariable::isPrintOutput())
                    {
                        //TODO manage multiple returns
                        scilabWriteW(L" ans  =\n\n");
                        std::wostringstream ostrName;
                        ostrName << L"ans";
                        types::VariableToString(pITAns, ostrName.str().c_str());
                    }
                }

                pIT->killMe();
            }

            if ((&e)->isBreakable() && exp->isBreak())
            {
                const_cast<SeqExp *>(&e)->setBreak();
                exp->resetBreak();
                break;
            }

            if ((&e)->isContinuable() && exp->isContinue())
            {
                const_cast<SeqExp *>(&e)->setContinue();
                exp->resetContinue();
                break;
            }

            if ((&e)->isReturnable() && exp->isReturn())
            {
                const_cast<SeqExp *>(&e)->setReturn();
                exp->resetReturn();
                break;
            }
        }
        catch (const InternalError& ie)
        {
            ConfigVariable::fillWhereError(ie.GetErrorLocation().first_line);

            std::vector<ConfigVariable::WhereEntry> lWhereAmI = ConfigVariable::getWhere();

            //where can be empty on last stepout, on first calling expression
            if (lWhereAmI.size())
            {
                std::wstring& filename = lWhereAmI.back().m_file_name;

                if (filename.empty())
                {
                    //error in a console script
                    std::wstring functionName = lWhereAmI.back().m_name;
                    manager->errorInScript(functionName, exp);
                }
                else
                {
                    manager->errorInFile(filename, exp);
                }
            }

            throw ie;
        }

        // If something other than NULL is given to setResult, then that would imply
        // to make a cleanup in visit(ForExp) for example (e.getBody().accept(*this);)
        setResult(NULL);

    }

    //propagate StepNext to parent SeqExp
    if (ConfigVariable::getEnableDebug())
    {
    }
}
}

