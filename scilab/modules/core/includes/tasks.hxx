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

#ifndef __TASKS_HXX__
#define __TASKS_HXX__

#include "parser.hxx"

/*
** Parse
**
** Parse the given file and create the AST.
*/
void parseFileTask(Parser *parser, bool timed, const char* file_name, const char* prog_name);

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
void execAstTask(ast::Exp *tree, bool timed, bool ASTtimed);

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
void execScilabStartTask(void);

#endif /* !__TASKS_HXX__ */
