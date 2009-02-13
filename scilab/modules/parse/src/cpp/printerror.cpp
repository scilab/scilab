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
#include "parser.hxx"

void Parser::PrintError(std::string msg) {
 int i = 0;

 // std::ifstream file(Parser::getInstance()->getFileName()->c_str());

 char *codeLine = NULL;

 /** First print where in the script the error is located */
 std::cerr << "[" <<*(Parser::getInstance()->getProgName()) << "] ";
 std::cerr << Parser::getInstance()->getCodeLine(yylloc.first_line, &codeLine) << std::endl;

 /** Then underline what causes the trouble */
 std::cerr << "[" <<*(Parser::getInstance()->getProgName()) << "] ";
 for( i = 1 ; i < yylloc.first_column ; ++i) {
   std::cerr << " ";
 }
 std::cerr << "^";
 for( i = i + 1 ; i < yylloc.last_column ; ++i) {
   std::cerr << "~";
 }
 if( yylloc.first_column != yylloc.last_column ) {
   std::cerr << "^" ;
 }
 std::cerr << std::endl;

 /** Finally display the Lexer / Parser message */
 std::cerr << "[" <<*(Parser::getInstance()->getProgName()) << "] ";
 std::cerr << *(Parser::getInstance()->getFileName()) << " : " <<
   yylloc.first_line << "." << yylloc.first_column <<
   " - " <<
   yylloc.last_line << "." << yylloc.last_column <<
   " : "<< msg << std::endl;
}
