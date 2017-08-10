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

#ifndef __TASKS_HXX__
#define __TASKS_HXX__

#include "parser.hxx"

extern "C"
{
#include "storeCommand.h" // command_origin_t
}

/*
** Parse
**
** Parse the given file and create the AST.
*/
void parseFileTask(Parser *parser, bool timed, const wchar_t* file_name, const wchar_t* prog_name);

/*
** Parse
**
** Parse the given command and create the AST.
*/
void parseCommandTask(Parser *parser, bool timed, char *command);

/*
** Dump AST
**
** Display the AST in human readable format.
*/
void dumpAstTask(ast::Exp *tree, bool timed);

/*
** Pretty Print
**
** Pretty print the Stored AST.
*/
void printAstTask(ast::Exp *tree, bool timed);

/*
** Exec Tree
**
** Execute the stored AST.
*/
void execAstTask(ast::Exp *tree, bool serialize, bool timed, bool ASTtimed, bool execVerbose,
                 bool _isInterruptibleThread, bool _isPrioritaryThread, command_origin_t _iCommandOrigin);

/*
** Exec Tree with original visitor ( without template )
**
** Execute the stored AST.
*/
void origAstTask(ast::Exp *tree, bool timed);

/*
** Dump Stack Trace
**
** Display what is stored in scilab.
*/
void dumpStackTask(bool timed);

/*
** Execute scilab.start
**
*/
int execScilabStartTask(bool _bSerialize);
int execScilabQuitTask(bool _bSerialize);

#ifdef __cplusplus
extern "C"
{
ast::Exp* parseCommand(std::wstring _command);
}
#endif


#endif /* !__TASKS_HXX__ */
