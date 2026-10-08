/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
#include "parser_private.hxx"
#include "visitor_common.hxx"

extern "C"
{
#include "FileExist.h"
}

namespace ast
{

void DebuggerVisitor::visit(const CallExp &e)
{
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    if(manager->isAborted())
    {
        // abort a running execution
        throw ast::InternalAbort();
    }

    visitprivate(e);

    if(manager->isStepOut())
    {
        if(manager->getSourceFile())
        {
            manager->resetStepOut();
            manager->stop(&e, -1);
        }

        if (manager->isAborted())
        {
            throw ast::InternalAbort();
        }
    }
}

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

        if (exp->isArgumentsExp())
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
        if (manager->isInterrupted() == false) // avoid stopping execution if an execution is already paused
        {
            bool stopExecution = false;
            bool hasSourceFile = manager->getSourceFile();
            if (hasSourceFile && manager->isStepIn())
            {
                manager->resetStepIn();
                stopExecution = true;
            }
            else if (hasSourceFile && manager->isStepNext())
            {
                manager->resetStepNext();
                stopExecution = true;
            }
            else
            {
                const std::vector<ConfigVariable::WhereEntry>& lWhereAmI = ConfigVariable::getWhere();
                //set information from debugger commands
                if (lWhereAmI.size() != 0 && manager->getBreakPointCount() != 0)
                {
                    manager->lockBreakpoints();
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
                            int iBPLine = bp->getFileLine();
                            if (iBPLine == exp->getLocation().first_line || iBPLine == -1)
                            {
                                std::string pFileName;
                                bool hasSource = manager->getSourceFile(&pFileName);
                                if (hasSource && bp->getFileName() == pFileName)
                                {
                                    stopExecution = true;
                                    
                                    if(iBPLine == -1)
                                    {
                                        // one shot breakpoint, remove it
                                        // used for stop on entry
                                        manager->removeBreakPoint(i);
                                    }
                                    else if (lWhereAmI.back().call->getFirstLine())
                                    {
                                        // set function information
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
                                if (pIT == NULL)
                                {
                                    // no result ie: assignation
                                    char pcError[256];
                                    sprintf(pcError, _("Wrong breakpoint condition: A result expected.\n"));
                                    bp->setConditionError(pcError);
                                }
                                else if(pIT->isTrue() == false)
                                {
                                    // bool scalar false
                                    pCtx->scope_end();
                                    stopExecution = false;
                                    continue;
                                }

                                // condition is invalid or true
                            }
                            catch (ast::ScilabException& e)
                            {
                                //not work !
                                //invalid breakpoint
                                if(ConfigVariable::isError())
                                {
                                    bp->setConditionError(scilab::UTF8::toUTF8(ConfigVariable::getLastErrorMessage()));
                                    ConfigVariable::clearLastError();
                                    ConfigVariable::resetError();
                                }
                                else
                                {
                                    bp->setConditionError(scilab::UTF8::toUTF8(e.GetErrorMessage()));
                                }
                            }

                            pCtx->scope_end();
                        }

                        // check hit condition
                        if(bp->hit() == false)
                        {
                            // hit condition not yet filled
                            stopExecution = false;
                            continue;
                        }

                        // reset hit counter
                        bp->resetHitCount();

                        //we have a breakpoint !
                        //stop execution and wait signal from debugger to restart
                        iBreakPoint = i;

                        //only one breakpoint can be "call" on same exp
                        break;
                    }
                    manager->unlockBreakpoints();
                }
            }

            // Ctrl+C: as the Pause button
            if (ConfigVariable::isExecutionBreak())
            {
                ConfigVariable::resetExecutionBreak();
                stopExecution = true;
            }

            if(stopExecution || manager->isPauseRequested())
            {
                manager->resetPauseRequest();
                manager->stop(exp, iBreakPoint);
                if (manager->isAborted())
                {
                    throw ast::InternalAbort();
                }
            }
        }

        // interrupt me to execute a prioritary command
        runPendingCommands(exp->getLocation());

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

            if (pIT != NULL)
            {
                bool bImplicitCall = false;
                bool isLambda = exp->isFunctionDec() && exp->getAs<ast::FunctionDec>()->isLambda();
                if (pIT->isCallable() && isLambda == false)
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
                        throw ie;
                    }
                }
                else if (pIT->isImplicitList())
                {
                    //expand implicit when possible
                    types::ImplicitList* pIL = pIT->getAs<types::ImplicitList>();
                    if (pIL->isComputable())
                    {
                        types::InternalType* p = pIL->extractFullMatrix();
                        if (p)
                        {
                            setResult(p);
                        }
                    }
                }

                //don't output Simplevar and empty result
                if (getResult() != NULL)
                {
                    setLambdaResult(getResult());
                    if (!exp->isSimpleVar() || bImplicitCall)
                    {
                        //symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *execMe.getResult());
                        types::InternalType* pITAns = getResult();
                        symbol::Context::getInstance()->put(m_pAns, pITAns);
                        if (exp->isVerbose() && ConfigVariable::isPrintOutput())
                        {
                            //TODO manage multiple returns
                            std::wostringstream ostrName;
                            ostrName << L"ans";
                            scilabWriteW(printVarEqualTypeDimsInfo(pITAns, L"ans").c_str());
                            types::VariableToString(pITAns, ostrName.str().c_str());
                        }
                    }
                }
                pIT->killMe();
            }

            // "pause" was called by this instruction: stop on it, still in
            // its scope even when it is the last one of a function
            if (manager->isPauseHereRequested())
            {
                manager->resetPauseRequest();
                manager->stop(exp, -1);
                if (manager->isAborted())
                {
                    throw ast::InternalAbort();
                }
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
                if(manager->getSourceFile())
                {
                    manager->resetStepOut();
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
            // dont manage an error with the debugger
            // in cases of try catch and errcatch
            if(ConfigVariable::isSilentError())
            {
                throw ie;
            }

            ConfigVariable::fillWhereError(ie.GetErrorLocation());

            // The stop consumes the call stack of the error, and leaving it
            // abandons the execution without going back to the one that
            // displays an error: it is displayed from here.
            std::wostringstream whereError;
            ConfigVariable::whereErrorToString(whereError);
            auto displayError = [&whereError, &ie]()
            {
                scilabErrorW(whereError.str().c_str());
                scilabErrorW(ie.GetErrorMessage().c_str());
            };

            const auto& lWhereError = ConfigVariable::getWhereError();

            //where can be empty on last stepout or on first calling expression
            if (lWhereError.size())
            {
                const std::wstring filename = lWhereError[0].m_file_name;

                try
                {
                    if (filename.empty())
                    {
                        //error in a console script
                        std::wstring functionName = lWhereError[0].m_function_name;
                        manager->errorInScript(functionName, exp);
                    }
                    else
                    {
                        manager->errorInFile(filename, exp);
                    }
                }
                catch (const ast::InternalAbort& ia)
                {
                    displayError();
                    throw ia;
                }
            }

            // Debugger just restart after been stopped on an error.
            if (manager->isAborted())
            {
                displayError();
                throw ast::InternalAbort();
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
                // reset step Next/In before stop
                // next action will be set when restart the execution
                manager->resetStepNext();
                manager->resetStepIn();
                manager->stop(&fakeExp, -1);

                if (manager->isAborted())
                {
                    throw ast::InternalAbort();
                }

                //transform stepnext/stepin after endfunction as a stepout to show line marker on current statement
                if (manager->isStepNext() || manager->isStepIn())
                {
                    manager->setStepOut();
                }
            }
        }
    }
}
}
