/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
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
