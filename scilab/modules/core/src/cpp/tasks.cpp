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
#include "originalvisitor.hxx"
#include "timedvisitor.hxx"
#include "debugvisitor.hxx"
#include "configvariable.hxx"

#include "yaspio.hxx"

#define SCILAB_START "/etc/scilab.start"

Timer _timer;

//#define DEBUG

/*
** Parse
**
** Parse the given file and create the AST.
*/
Parser::ParserStatus parseFileTask(bool timed, const char* file_name, const char* prog_name)
{
#ifdef DEBUG
    std::cerr << "*** Processing " << file_name << " file..." << std::endl;
#endif

    if(timed)
    {
        _timer.start();
    }

    Parser::getInstance()->parseFile(file_name, prog_name);

    if(timed)
    {
        _timer.check("Parsing");
    }

    return (Parser::getInstance()->getExitStatus());
}

/*
** Parse
**
** Parse the given command and create the AST.
*/
Parser::ParserStatus parseCommandTask(bool timed, char *command)
{
#ifdef DEBUG
    std::cerr << "*** Processing [" <<  command << "]..." << std::endl;
#endif

    if(timed)
    {
        _timer.start();
    }

    Parser::getInstance()->parse(command);

    if(timed && Parser::getInstance()->getControlStatus() == Parser::AllControlClosed)
    {
        _timer.check("Parsing");
    }

    return (Parser::getInstance()->getExitStatus());
}

/*
** Dump AST
**
** Display the AST in human readable format.
*/
void dumpAstTask(bool timed)
{
    if(timed)
    {
        _timer.start();
    }

    ast::DebugVisitor debugMe;
    if (Parser::getInstance()->getTree())
    {
        Parser::getInstance()->getTree()->accept(debugMe);
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
void printAstTask(bool timed)
{
    if(timed)
    {
        _timer.start();
    }

    ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);
    Parser::getInstance()->getTree()->accept(printMe);

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
void execAstTask(bool timed, bool ASTtimed)
{
    ast::ExecVisitor *exec;
    if(timed)
    {
        _timer.start();
    }

    if(ASTtimed)
    {
        exec = (ast::ExecVisitor*)new ast::TimedVisitor();
    }
    else
    {
        exec = new ast::ExecVisitor();
    }

    try
    {
        Parser::getInstance()->getTree()->accept(*exec);
    }
    catch(string sz)
    {
        YaspWrite((char *) sz.c_str());
        YaspWrite("\n");
    }

    delete exec;

    if(timed)
    {
        _timer.check("Execute AST");
    }
}

/*
** Exec Tree with original visitor ( without template )
**
** Execute the stored AST.
*/
void origAstTask(bool timed)
{
    ast::OriginalVisitor exec;
    if(timed)
    {
        _timer.start();
    }

    try
    {
        Parser::getInstance()->getTree()->accept(exec);
    }
    catch(string sz)
    {
        YaspWrite((char *) sz.c_str());
        YaspWrite("\n");
    }

    if(timed)
    {
        _timer.check("Execute original AST");
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

    symbol::Context::getInstance()->print();

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
    return;
    Parser* pParse = Parser::getInstance();
    string stSCI = ConfigVariable::getInstance()->get("SCI");
    stSCI += SCILAB_START;
    pParse->parseFile(stSCI, "");

    if(pParse->getExitStatus() != Parser::Succeded)
    {
        YaspWrite(pParse->getErrorMessage());
        YaspWrite("Failed to parse scilab.start");
        return;
    }

    execAstTask(false, false);
}
