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
#include "macrofile.hxx"
#include "commentexp.hxx"
#include "UTF8.hxx"
#include "runner.hxx"

extern "C"
{
#include "filemanager_interface.h"
#include "FileExist.h"
}

static bool getMacroSourceFile(std::string* filename = nullptr);

namespace ast
{
void DebuggerVisitor::visit(const SeqExp  &e)
{
    std::list<Exp *>::const_iterator itExp;

    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    if(manager->isAborted())
    {
        // abort a running execution
        throw ast::InternalAbort();
    }

    for (const auto & exp : e.getExps())
    {
        if (exp->isCommentExp())
        {
            continue;
        }

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
        int iBreakPoint = -1;
        if (ConfigVariable::getEnableDebug())
        {
            bool stopExecution = false;
            if (manager->isStepIn())
            {
                manager->resetStepIn();
                stopExecution = true;
                if(getMacroSourceFile() == false)
                {
                    stopExecution = false;
                    manager->setStepIn();
                }
            }
            else if (manager->isStepNext())
            {
                manager->resetStepNext();
                stopExecution = true;
                if(getMacroSourceFile() == false)
                {
                    stopExecution = false;
                    manager->setStepOut();
                }
            }
            else
            {
                const std::vector<ConfigVariable::WhereEntry>& lWhereAmI = ConfigVariable::getWhere();
                //set information from debugger commands
                if (lWhereAmI.size() != 0 && manager->getBreakPointCount() != 0)
                {
                    debugger::Breakpoints& bps = manager->getAllBreakPoint();

                    int i = -1;
                    for (const auto & bp : bps)
                    {
                        ++i;
                        if (bp->isEnable() == false)
                        {
                            continue;
                        }

                        // look for a breakpoint on this line and update breakpoint information when possible
                        int iLine = exp->getLocation().first_line - ConfigVariable::getMacroFirstLines();
                        if (bp->hasMacro())
                        {
                            char* functionName = wide_string_to_UTF8(lWhereAmI.back().call->getName().data());
                            if (bp->getFunctioName().compare(functionName) == 0)
                            {
                                if (bp->getMacroLine() == 0)
                                {
                                    //first pass in macro.
                                    //update first line with real value
                                    bp->setMacroLine(iLine);
                                }

                                stopExecution = bp->getMacroLine() == iLine;
                            }

                            FREE(functionName);
                        }
                        else if (bp->hasFile())
                        {
                            if (bp->getFileLine() == exp->getLocation().first_line)
                            {
                                std::string pFileName;
                                bool hasSource = getMacroSourceFile(&pFileName);
                                if (hasSource && bp->getFileName() == pFileName)
                                {
                                    stopExecution = true;
                                    // set function information
                                    if (lWhereAmI.back().call->getFirstLine())
                                    {
                                        bp->setFunctionName(scilab::UTF8::toUTF8(lWhereAmI.back().call->getName()));
                                        bp->setMacroLine(iLine);
                                    }
                                }
                            }
                        }

                        if(stopExecution == false)
                        {
                            // no breakpoint for this line
                            continue;
                        }

                        // Set the begin of line if not yet done
                        if(bp->getBeginLine() == 0)
                        {
                            bp->setBeginLine(exp->getLocation().first_column);
                        }
                        // check if this exp is at the begin of the breakpoint line
                        else if(bp->getBeginLine() != exp->getLocation().first_column)
                        {
                            // stop only if we are at the line begins
                            stopExecution = false;
                            continue;
                        }

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
                                    stopExecution = false;
                                    continue;
                                }

                                pCtx->scope_end();
                                //ok condition is valid and true
                            }
                            catch (ast::ScilabException &/*e*/)
                            {
                                pCtx->scope_end();
                                stopExecution = false;
                                //not work !
                                //invalid breakpoint
                                continue;
                            }
                        }

                        //we have a breakpoint !
                        //stop execution and wait signal from debugger to restart
                        iBreakPoint = i;

                        //only one breakpoint can be "call" on same exp
                        break;
                    }
                }
            }

            if(stopExecution)
            {
                manager->stop(exp, iBreakPoint);
                if (manager->isAborted())
                {
                    throw ast::InternalAbort();
                }
            }
        }

        // interrupt me to execute a prioritary command
        while (StaticRunner_isRunnerAvailable() == 1 && StaticRunner_isInterruptibleCommand() == 1)
        {
            StaticRunner_launch();
        }

        //copy from runvisitor::seqexp
        try
        {
            //reset default values
            setResult(NULL);
            int iExpectedSize = getExpectedSize();
            setExpectedSize(-1);
            exp->accept(*this);
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
                        //in this case of calling, we can return only one value
                        int iSaveExpectedSize = getExpectedSize();
                        setExpectedSize(1);

                        pCall->invoke(in, opt, getExpectedSize(), out, *exp);
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

            // Stop execution at the end of the seqexp of the caller
            // Do it at the end of the seqexp will make the debugger stop
            // even if the caller is at the last line
            // ie: the caller is followed by endfunction
            if(manager->isStepOut())
            {
                manager->resetStepOut();
                if(getMacroSourceFile() == false)
                {
                    // no sources
                    manager->setStepOut();
                }
                else
                {
                    manager->stop(exp, iBreakPoint);
                }

                if (manager->isAborted())
                {
                    throw ast::InternalAbort();
                }
            }

        }
        catch (const InternalError& ie)
        {
            ConfigVariable::fillWhereError(ie.GetErrorLocation().first_line);

            const std::vector<ConfigVariable::WhereEntry>& lWhereAmI = ConfigVariable::getWhere();

            //where can be empty on last stepout, on first calling expression
            if (lWhereAmI.size())
            {
                const std::wstring* filename = lWhereAmI.back().m_file_name;

                if (filename == nullptr)
                {
                    //error in a console script
                    std::wstring functionName = lWhereAmI.back().call->getName();
                    manager->errorInScript(functionName, exp);
                }
                else
                {
                    manager->errorInFile(*filename, exp);
                }

                // Debugger just restart after been stopped on an error.
                if (manager->isAborted())
                {
                    throw ast::InternalAbort();
                }
            }

            throw ie;
        }

        // If something other than NULL is given to setResult, then that would imply
        // to make a cleanup in visit(ForExp) for example (e.getBody().accept(*this);)
        setResult(NULL);

    }

    if (e.getParent() == NULL && e.getExecFrom() == SeqExp::SCRIPT && (manager->isStepNext() || manager->isStepIn()))
    {
        const std::vector<ConfigVariable::WhereEntry>& lWhereAmI = ConfigVariable::getWhere();
        if (lWhereAmI.size())
        {
            std::wstring functionName = lWhereAmI.back().call->getName();
            types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(functionName));
            if (pIT && (pIT->isMacro() || pIT->isMacroFile()))
            {
                types::Macro* m = nullptr;
                if (pIT->isMacroFile())
                {
                    types::MacroFile* mf = pIT->getAs<types::MacroFile>();
                    m = mf->getMacro();
                }
                else
                {
                    m = pIT->getAs<types::Macro>();
                }

                //create a fake exp to represente end/enfunction

                //will be deleted by CommentExp
                std::wstring* comment = new std::wstring(L"end of function");
                Location loc(m->getLastLine(), m->getLastLine(), 0, 0);
                CommentExp fakeExp(loc, comment);
                manager->stop(&fakeExp, -1);

                if (manager->isAborted())
                {
                    throw ast::InternalAbort();
                }

                //transform stepnext after endfunction as a stepout to show line marker on current statement
                if (manager->isStepNext())
                {
                    manager->resetStepNext();
                    manager->setStepOut();
                }
                else if (manager->isStepIn())
                {
                    manager->resetStepIn();
                    manager->setStepOut();
                }
            }
        }
    }
}
}

// return false if a file .sci of a file .bin doesn't exists
// return true for others files or existing .sci
bool getMacroSourceFile(std::string* filename)
{
    const std::vector<ConfigVariable::WhereEntry>& lWhereAmI = ConfigVariable::getWhere();
    std::string file = scilab::UTF8::toUTF8(*lWhereAmI.back().m_file_name);
    if (file.rfind(".bin") != std::string::npos)
    {
        file.replace(file.size() - 4, 4, ".sci");
        // stop on bp only if the file exist
        if (!FileExist(file.data()))
        {
            return false;
        }
    }

    if(filename != nullptr)
    {
        filename->assign(file);
    }

    return true;
}