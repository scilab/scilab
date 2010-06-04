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

void ParserSingleInstance::PrintError(std::string msg) {
 int i = 0;

 // FIXME : Should work under Windows
 // Need to have getline !!!
 std::ostringstream ostr;
#ifndef _MSC_VER
 char *codeLine = NULL;

 /** First print where in the script the error is located */
 ostr << "[" <<*(ParserSingleInstance::getProgName()) << "] ";

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
 ostr << "[" <<*(ParserSingleInstance::getProgName()) << "] ";
 for( i = 1 ; i < yylloc.first_column ; ++i) {
   ostr << " ";
 }
 ostr << "^";
 for( i = i + 1 ; i < yylloc.last_column ; ++i) {
   ostr << "~";
 }
 if( yylloc.first_column != yylloc.last_column ) {
   ostr << "^" ;
 }
 ostr << std::endl;
#endif

 /** Finally display the Lexer / Parser message */
 ostr << "[" <<*(ParserSingleInstance::getProgName()) << "] ";
 ostr << *(ParserSingleInstance::getFileName()) << " : " <<
   yylloc.first_line << "." << yylloc.first_column <<
   " - " <<
   yylloc.last_line << "." << yylloc.last_column <<
   " : "<< msg << std::endl;


 //yylloc.first_line -= yylloc.last_line;
 //yylloc.last_line = yylloc.first_line;
 //yylloc.last_column = yylloc.first_column;
 
 ParserSingleInstance::appendErrorMessage(ostr.str());
}
