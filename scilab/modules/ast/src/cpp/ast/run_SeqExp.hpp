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

#include <fstream>
//file included in runvisitor.cpp
namespace ast {

template <class T>
void RunVisitorT<T>::visitprivate(const SeqExp  &e)
{
    CoverageInstance::invokeAndStartChrono((void*)&e);
    int lastLine = 0;
    ast::exps_t exps = e.getExps();
    ast::exps_t::const_iterator it = exps.begin();
    ast::exps_t::const_iterator itEnd = exps.end();


    /*stuff of printf input during execution*/
    std::string str;
    int iCurrentLine = -1; //no data in str

    std::ifstream* file = nullptr;
    if (e.getExecFrom() == SeqExp::EXEC)
    {
        //open input file to print exp from it
        int iFileID = ConfigVariable::getExecutedFileID();
        if (iFileID)
        {
            const wchar_t* filename = getfile_filename(iFileID);
            if (filename)
            {
                char* cfilename = wide_string_to_UTF8(filename);
                file = new std::ifstream(cfilename);
                FREE(cfilename);
            }
        }
    }

    for (; it != itEnd; ++it)
    {
        if (ConfigVariable::isExecutionBreak())
        {
            ConfigVariable::resetExecutionBreak();
            if (ConfigVariable::isPrintInteractive())
            {
                ClearTemporaryPrompt();
            }

            StorePrioritaryCommand("pause");
            ThreadManagement::WaitForRunMeSignal();
        }

        // interrupt me to execute a prioritary command
        while (StaticRunner_isInterruptibleCommand() == 1 && StaticRunner_isRunnerAvailable() == 1)
        {
            StaticRunner_launch();
            StaticRunner_setInterruptibleCommand(1);
        }

        //printf input expression line following mode configuration
        if (file && ConfigVariable::isPrintInput())
        {
            Location loc = (*it)->getLocation();
            if (iCurrentLine + 1 < loc.first_line)
            {
                char pstPrompt[64];
                //get prompt
                GetCurrentPrompt(pstPrompt);
                std::string stPrompt(pstPrompt);

                if (ConfigVariable::isPrintInteractive())
                {
                    stPrompt = SCIPROMPT_PAUSE;
                }

                ast::exps_t::const_iterator k = it;
                int iLastLine = loc.last_line;
                int iCurrentCol = 0; //no data in str
                do
                {
                    str = printExp(*file, *k, stPrompt, &iCurrentLine, &iCurrentCol, str);
                    iLastLine = (*k)->getLocation().last_line;
                    k++;
                } while (k != exps.end() && (*k)->getLocation().first_line == iLastLine);
            }
        }

        if ((*it)->isCommentExp())
        {
            continue;
        }

        try
        {
            //reset default values
            setResult(NULL);
            int iExpectedSize = getExpectedSize();
            setExpectedSize(-1);
            (*it)->accept(*this);
            setExpectedSize(iExpectedSize);
            types::InternalType * pIT = getResult();

            // In case of exec file, set the file name in the Macro to store where it is defined.
            int iFileID = ConfigVariable::getExecutedFileID();
            if (iFileID && (*it)->isFunctionDec())
            {
                types::InternalType* pITMacro = symbol::Context::getInstance()->get((*it)->getAs<FunctionDec>()->getSymbol());
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
                        CoverageInstance::stopChrono((void*)&e);
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
                if (getResult() != NULL && (!(*it)->isSimpleVar() || bImplicitCall))
                {
                    //symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *execMe.getResult());
                    types::InternalType* pITAns = getResult();
                    symbol::Context::getInstance()->put(m_pAns, pITAns);
                    if ((*it)->isVerbose() && ConfigVariable::isPrintOutput())
                    {
                        //TODO manage multiple returns
                        scilabWriteW(L" ans  =\n\n");
                        std::wostringstream ostrName;
                        ostrName << L"ans";
                        VariableToString(pITAns, ostrName.str().c_str());
                    }
                }

                pIT->killMe();
            }

            if (ConfigVariable::isPrintInteractive())
            {
                Location loc = (*it)->getLocation();
                if (lastLine < loc.first_line)
                {
                    //break execution
                    SetTemporaryPrompt(SCIPROMPT_PAUSE);

                    // The console thread must not parse the next console input.
                    ConfigVariable::setScilabCommand(0);

                    // Get the console input filled by the console thread.
                    char* pcConsoleReadStr = ConfigVariable::getConsoleReadStr();
                    ThreadManagement::SendConsoleExecDoneSignal();
                    while (pcConsoleReadStr == NULL)
                    {
                        pcConsoleReadStr = ConfigVariable::getConsoleReadStr();
                        ThreadManagement::SendConsoleExecDoneSignal();
                    }

                    // reset flag to default value
                    ConfigVariable::setScilabCommand(1);

                    if (pcConsoleReadStr && pcConsoleReadStr[0] == 'p' && pcConsoleReadStr[1] == '\0')
                    {
                        //mode pause
                        ConfigVariable::setExecutionBreak();
                    }
                }

                lastLine = loc.last_line;
            }

            if ((&e)->isBreakable() && (*it)->isBreak())
            {
                const_cast<SeqExp *>(&e)->setBreak();
                (*it)->resetBreak();
                break;
            }

            if ((&e)->isContinuable() && (*it)->isContinue())
            {
                const_cast<SeqExp *>(&e)->setContinue();
                (*it)->resetContinue();
                break;
            }

            if ((&e)->isReturnable() && (*it)->isReturn())
            {
                const_cast<SeqExp *>(&e)->setReturn();
                (*it)->resetReturn();
                break;
            }
        }
        catch (const InternalError& ie)
        {
            ConfigVariable::fillWhereError(ie.GetErrorLocation().first_line);
            CoverageInstance::stopChrono((void*)&e);
            if (file)
            {
                file->close();
                delete file;
            }

            throw ie;
        }
        catch (const InternalAbort& ia)
        {
            if (file)
            {
                file->close();
                delete file;
            }
            throw ia;
        }
        // If something other than NULL is given to setResult, then that would imply
        // to make a cleanup in visit(ForExp) for example (e.getBody().accept(*this);)
        setResult(NULL);
    }

    if (file)
    {
        file->close();
        delete file;
    }
    CoverageInstance::stopChrono((void*)&e);
}


} /* namespace ast */
