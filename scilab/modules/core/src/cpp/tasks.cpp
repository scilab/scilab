/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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

//#include "AnalysisVisitor.hxx"
#include "tasks.hxx"
#include "timer.hxx"
#include "context.hxx"
#include "visitor.hxx"
#include "printvisitor.hxx"
#include "execvisitor.hxx"
#include "timedvisitor.hxx"
#include "prettyprintvisitor.hxx"
#include "debuggervisitor.hxx"
#include "stepvisitor.hxx"
#include "visitor_common.hxx"
#include "threadmanagement.hxx"

#include "scilabWrite.hxx"
#include "runner.hxx"

#define SCILAB_START    L"/etc/scilab.start"
#define SCILAB_QUIT     L"/etc/scilab.quit"

Timer _timer;

//#define DEBUG

/*
** Parse
**
** Parse the given file and create the AST.
*/
void parseFileTask(Parser *parser, bool timed, const wchar_t* file_name, const wchar_t* prog_name)
{
#ifdef DEBUG
    std::cerr << "*** Processing " << file_name << " file..." << std::endl;
#endif

    if (timed)
    {
        _timer.start();
    }

    parser->parseFile(file_name, prog_name);

    if (timed)
    {
        _timer.check(L"Parsing");
    }
}

/*
** Parse
**
** Parse the given command and create the AST.
*/
void parseCommandTask(Parser *parser, bool timed, char *command)
{
#ifdef DEBUG
    std::cerr << "*** Processing [" <<  command << "]..." << std::endl;
#endif

    if (timed)
    {
        _timer.start();
    }

    parser->parse(command);

    if (timed && parser->getControlStatus() == Parser::AllControlClosed)
    {
        _timer.check(L"Parsing");
    }
}

/*
** Dump AST
**
** Display the AST in human readable format.
*/
void dumpAstTask(ast::Exp *tree, bool timed)
{
    if (timed)
    {
        _timer.start();
    }

    ast::PrettyPrintVisitor debugMe;
    if (tree)
    {
        tree->accept(debugMe);
    }

    if (timed)
    {
        _timer.check(L"AST Dump");
    }
}

/*
** Pretty Print
**
** Pretty print the Stored AST.
*/
void printAstTask(ast::Exp *tree, bool timed)
{
    if (timed)
    {
        _timer.start();
    }

    if (tree)
    {
        ast::PrintVisitor printMe (std::wcout);
        tree->accept(printMe);
    }

    if (timed)
    {
        _timer.check(L"Pretty Print");
    }
}


/*
** Exec Tree
**
** Execute the stored AST.
*/
void execAstTask(ast::Exp* tree, bool serialize, bool timed, bool ASTtimed, bool execVerbose,
                 bool isInterruptibleThread, bool isPrioritaryThread, command_origin_t iCommandOrigin)
{
    if (tree == NULL)
    {
        return;
    }

    ast::Exp* newTree = NULL;
    if (serialize)
    {
        if (timed)
        {
            newTree = callTyper(tree, L"tasks");
        }
        else
        {
            newTree = callTyper(tree);
        }

        delete tree;
    }
    else
    {
        newTree = tree;
    }

    ast::RunVisitor *exec;
    if (timed)
    {
        _timer.start();
    }

    if (ASTtimed)
    {
        exec = new ast::TimedVisitor();
    }
    else if (execVerbose)
    {
        exec = new ast::StepVisitor();
    }
    else
    {
        //call analyzer visitor before exec visitor
        if (ConfigVariable::getAnalyzerOptions() == 1)
        {
            //analysis::AnalysisVisitor analysis;
            //newTree->accept(analysis);
        }

        exec = (ast::RunVisitor*)ConfigVariable::getDefaultVisitor();
    }

    StaticRunner::execAndWait(newTree, exec, isInterruptibleThread, isPrioritaryThread, iCommandOrigin);
    //DO NOT DELETE tree or newTree, they was deleted by Runner or previously;

    if (timed)
    {
        _timer.check(L"Execute AST");
    }
}

/*
** Dump Stack Trace
**
** Display what is stored in scilab.
*/
void dumpStackTask(bool timed)
{
    if (timed)
    {
        _timer.start();
    }

    symbol::Context::getInstance()->print(std::wcout);

    if (timed)
    {
        _timer.check(L"Dumping Stack");
    }
}

/*
** Execute scilab.start
**
*/
int execScilabStartTask(bool _bSerialize)
{
    Parser parse;
    std::wstring stSCI = ConfigVariable::getSCIPath();
    stSCI += SCILAB_START;

    ThreadManagement::LockParser();
    try
    {
        parse.parseFile(stSCI, L"");
    }
    catch (const ast::InternalError& ie)
    {
        scilabWrite(ie.what());
        ThreadManagement::UnlockParser();
        return 1;
    }

    if (parse.getExitStatus() != Parser::Succeded)
    {
        scilabWriteW(parse.getErrorMessage());
        scilabWriteW(L"Failed to parse scilab.start");
        ThreadManagement::UnlockParser();
        return 1;
    }
    ThreadManagement::UnlockParser();

    ast::Exp* newTree = parse.getTree();
    if (_bSerialize)
    {
        newTree = callTyper(parse.getTree());
    }

    return StaticRunner::exec(newTree, new ast::ExecVisitor()) ? 0 : 1;
}

/*
** Execute scilab.quit
**
*/
int execScilabQuitTask(bool _bSerialize)
{
    Parser parse;
    std::wstring stSCI = ConfigVariable::getSCIPath();
    stSCI += SCILAB_QUIT;

    ThreadManagement::LockParser();
    try
    {
        parse.parseFile(stSCI, L"");
    }
    catch (const ast::InternalError& ie)
    {
        scilabWrite(ie.what());
        ThreadManagement::UnlockParser();
        return 1;
    }

    if (parse.getExitStatus() != Parser::Succeded)
    {
        scilabWriteW(parse.getErrorMessage());
        scilabWriteW(L"Failed to parse scilab.quit");
        ThreadManagement::UnlockParser();
        return 1;
    }
    ThreadManagement::UnlockParser();

    ast::Exp* newTree = parse.getTree();
    if (_bSerialize)
    {
        newTree = callTyper(parse.getTree());
    }

    return StaticRunner::exec(newTree, new ast::ExecVisitor()) ? 0 : 1;
}


ast::Exp* parseCommand(std::wstring _command)
{
    if (_command.empty())
    {
        return NULL;
    }

    Parser parse;
    parse.parse((wchar_t*)_command.c_str());
    if (parse.getExitStatus() != Parser::Succeded)
    {
        return NULL;
    }

    return parse.getTree();
}
