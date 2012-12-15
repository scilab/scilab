/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "tasks.hxx"
#include "timer.hxx"
#include "context.hxx"
#include "visitor.hxx"
#include "printvisitor.hxx"
#include "execvisitor.hxx"
#include "timedvisitor.hxx"
#include "debugvisitor.hxx"
#include "stepvisitor.hxx"
#include "configvariable.hxx"

#include "jit_ocaml.hxx"
extern "C" {
#include "jit_ocaml.h"
}

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

    if(timed)
    {
        _timer.start();
    }

    parser->parseFile(file_name, prog_name);

    if(timed)
    {
        _timer.check("Parsing");
    }
}

/*
** Parse
**
** Parse the given command and create the AST.
*/
void parseCommandTask(Parser *parser, bool timed, wchar_t *command)
{
#ifdef DEBUG
    std::cerr << "*** Processing [" <<  command << "]..." << std::endl;
#endif

    if(timed)
    {
        _timer.start();
    }

    parser->parse(command);

    if(timed && parser->getControlStatus() == Parser::AllControlClosed)
    {
        _timer.check("Parsing");
    }
}

/*
** Dump AST
**
** Display the AST in human readable format.
*/
void dumpAstTask(ast::Exp *tree, bool timed)
{
    if(timed)
    {
        _timer.start();
    }

    ast::DebugVisitor debugMe;
    if (tree)
    {
        tree->accept(debugMe);
    }

    if(timed)
    {
        _timer.check("AST Dump");
    }
}

/*
** Pretty Print
**
** Pretty print the Stored AST.
*/
void printAstTask(ast::Exp *tree, bool timed)
{
    if(timed)
    {
        _timer.start();
    }

    if (tree)
    {
        ast::PrintVisitor printMe = *new ast::PrintVisitor(std::wcout);
        tree->accept(printMe);
    }

    if(timed)
    {
        _timer.check("Pretty Print");
    }
}


/*
** Exec Tree
**
** Execute the stored AST.
*/
void execAstTask(ast::Exp* tree, bool timed, bool ASTtimed, bool execVerbose)
{
    if(tree == NULL)
    {
        return;
    }

    ast::ExecVisitor *exec;
    if(timed)
    {
        _timer.start();
    }

    char *buf = scicaml_ast2string(tree);
    buf = jit_ocaml_analyze(buf);
    ast::Exp* ocaml_tree = scicaml_string2ast(buf);
    /* uncomment these lines to verify that the translation is IDEMPOTENT

    char *buf2 = scicaml_ast2string(ocaml_tree);
    jit_ocaml_analyze(buf2);
    */
    if(ASTtimed)
    {
        exec = (ast::ExecVisitor*)new ast::TimedVisitor();
    }

    if(execVerbose)
    {
        exec = (ast::ExecVisitor*)new ast::StepVisitor();
    }

    if(!execVerbose && !ASTtimed)
    {
        exec = new ast::ExecVisitor();
    }

    Runner::execAndWait(ocaml_tree, exec);
    //delete exec;

    if(timed)
    {
        _timer.check("Execute AST");
    }
}

/*
** Dump Stack Trace
**
** Display what is stored in scilab.
*/
void dumpStackTask(bool timed)
{
    if(timed)
    {
        _timer.start();
    }

    symbol::Context::getInstance()->print(std::wcout);

    if(timed)
    {
        _timer.check("Dumping Stack");
    }
}

/*
** Execute scilab.start
**
*/
void execScilabStartTask(void)
{
    Parser parse;
    wstring stSCI = ConfigVariable::getSCIPath();

    stSCI += SCILAB_START;
    parse.parseFile(stSCI, L"");

    if(parse.getExitStatus() != Parser::Succeded)
    {
        scilabWriteW(parse.getErrorMessage());
        scilabWriteW(L"Failed to parse scilab.start");
        return;
    }

    execAstTask(parse.getTree(), false, false, false);
}

/*
** Execute scilab.start
**
*/
void execScilabQuitTask(void)
{
    Parser parse;
    wstring stSCI = ConfigVariable::getSCIPath();

    stSCI += SCILAB_QUIT;
    parse.parseFile(stSCI, L"");

    if(parse.getExitStatus() != Parser::Succeded)
    {
        scilabWriteW(parse.getErrorMessage());
        scilabWriteW(L"Failed to parse scilab.start");
        return;
    }

    execAstTask(parse.getTree(), false, false, false);
}


