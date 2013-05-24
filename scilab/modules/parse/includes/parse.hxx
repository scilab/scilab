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

/**
 ** \file parse.hh
 ** Declaration shared between the scanner and the parser but private
 ** to the parse module.
 */

#ifndef PARSE_HH_
#define PARSE_HH_

#include <string>
#include <iostream>
#include <errno.h>
#include "exit_status.hxx"
#include "all.hxx"


typedef struct __EXPRESSIONLINEBREAK__
{
    bool bVerbose;
    int iNbBreaker;
} LineBreakStr;


/**
define YYLTYPE to override Bison Default Location
*/
#define YYLTYPE Location
#include "parsescilab.hxx"

/** \brief Parsing entry . */
//int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);
int yylex(void);

/** \brief Parsing error . */
void yyerror(std::string msg);


/** \brief FIXME . */
int scan_throw(int);

int get_last_token();

/** \brief For scanner just step . */
void scan_step(void);

/** \brief Error from the scanner (from scanscilab) . */
void scan_error(std::string msg);

/** \brief Open the scanner step (from scanscilab) . */
void scan_open(const std::string &name);

/** \brief Close the scanner step (from scanscilab) . */
void scan_close(void);

/** \brief Convert exponent numbers to C standard . */
void scan_exponent_convert(char *in);

/** \brief Parse/Scan error message displat . */
void PrintError(std::string msg);

#endif /* !PARSE_HH_ */
