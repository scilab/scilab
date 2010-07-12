/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <fstream>
#include <string>
#include "parser_private.hxx"

void ParserSingleInstance::PrintError(std::wstring msg) {
 int i = 0;

 // FIXME : Should work under Windows
 // Need to have getline !!!
 std::wostringstream ostr;
#ifndef _MSC_VER
 char *codeLine = NULL;

 /** First print where in the script the error is located */
 ostr << L"[" << *(ParserSingleInstance::getProgName()) << L"] ";

 /*
 ** If the error is a the very beginning of a line
 */
 if (yylloc.first_line == yylloc.last_line
     && yylloc.first_column == 1
     && yylloc.last_column == 1)
 {
   --yylloc.first_line;
 }

 ostr << ParserSingleInstance::getCodeLine(yylloc.first_line, &codeLine) << std::endl;
 free(codeLine);

 /** Then underline what causes the trouble */
 ostr << L"[" << *(ParserSingleInstance::getProgName()) << L"] ";
 for( i = 1 ; i < yylloc.first_column ; ++i) {
   ostr << L" ";
 }
 ostr << L"^";
 for( i = i + 1 ; i < yylloc.last_column ; ++i) {
   ostr << L"~";
 }
 if( yylloc.first_column != yylloc.last_column ) {
   ostr << L"^" ;
 }
 ostr << std::endl;
#endif

 /** Finally display the Lexer / Parser message */
 ostr << L"[" <<*(ParserSingleInstance::getProgName()) << L"] ";
 ostr << *(ParserSingleInstance::getFileName()) << L" : " <<
   yylloc.first_line << L"." << yylloc.first_column <<
   L" - " <<
   yylloc.last_line << L"." << yylloc.last_column <<
   L" : " << msg << std::endl;


 //yylloc.first_line -= yylloc.last_line;
 //yylloc.last_line = yylloc.first_line;
 //yylloc.last_column = yylloc.first_column;
 
 ParserSingleInstance::appendErrorMessage(ostr.str());
}
