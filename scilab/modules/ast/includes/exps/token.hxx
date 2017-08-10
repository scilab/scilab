/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
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

#ifndef __AST_TOKEN_HXX__
#define __AST_TOKEN_HXX__

/** Scilab tokens and key words
 ** \{ */

#define SCI_DOT			            L"."

#define SCI_TRUE		            L"%T"
#define SCI_FALSE		            L"%F"

#define SCI_INT8		            L"int8"
#define SCI_INT16		            L"int16"
#define SCI_INT32		            L"int32"
#define SCI_INT64		            L"int64"
#define SCI_LPAREN		            L"("
#define SCI_RPAREN		            L")"
#define SCI_LBRACE		            L"{"
#define SCI_RBRACE		            L"}"
#define SCI_LBRACK		            L"["
#define SCI_RBRACK		            L"]"

#define SCI_PLUS		            L"+"
#define SCI_MINUS		            L"-"
#define SCI_TIMES		            L"*"
#define SCI_RDIVIDE		            L"/"
#define SCI_LDIVIDE		            L"\\"
#define SCI_POWER		            L"^"

#define SCI_DOTTIMES		        L".*"
#define SCI_DOTRDIVIDE		        L"./"
#define SCI_DOTLDIVIDE		        L".\\"
#define SCI_DOTPOWER		        L".^"

#define SCI_KRONTIMES		        L".*."
#define SCI_KRONRDIVIDE		        L"./."
#define SCI_KRONLDIVIDE		        L".\\."

#define SCI_CONTROLTIMES	        L"*."
#define SCI_CONTROLRDIVIDE	        L"/."
#define SCI_CONTROLLDIVIDE	        L"\\."

#define SCI_EQ			            L"=="
#define SCI_NE			            L"<>"
#define SCI_GE			            L">="
#define SCI_GT			            L">"
#define SCI_LE			            L"<="
#define SCI_LT			            L"<"

#define SCI_AND			            L"&"
#define SCI_OR			            L"|"
#define SCI_ANDAND	                L"&&"
#define SCI_OROR		            L"||"
#define SCI_NOT			            L"~"

#define SCI_CONJUGATE_TRANSPOSE	    L"'"
#define SCI_TRANSPOSE		        L".'"

#define SCI_QUOTE		            L"\'"
#define SCI_DQUOTE		            L"\""
#define SCI_COMMA		            L","
#define SCI_SEMICOLON	            L";"
#define SCI_COLON		            L":"

#define SCI_DOLLAR		            L"$"

#define SCI_ASSIGN		            L"="
#define SCI_OPEN_COMMENT	        L"//"

// Syntax to acces field 'bar' in variable 'foo'
// foo.var
#define SCI_FVAR_SEPARATOR          SCI_DOT

// Syntax to access indexed variable in 'foo'
// foo(5)
#define SCI_SVAR_OPEN		        SCI_LPAREN
#define SCI_SVAR_CLOSE		        SCI_RPAREN

// Syntax to do strings
#define SCI_OPEN_STRING		        SCI_DQUOTE
#define SCI_CLOSE_STRING	        SCI_DQUOTE

// Syntax to have function calls
#define SCI_OPEN_CALL		        SCI_LPAREN
#define SCI_CLOSE_CALL		        SCI_RPAREN
#define SCI_ARGS_SEPARATOR	        SCI_COMMA

// Syntax to have if control structure
#define SCI_IF			            L"if"
#define SCI_THEN		            L"then"
#define SCI_ELSE		            L"else"
#define SCI_ENDIF		            L"end"
#define SCI_OPEN_TEST		        SCI_LPAREN
#define SCI_CLOSE_TEST		        SCI_RPAREN

// Syntax to have while control structure
#define SCI_WHILE		            L"while"
#define SCI_DO			            L"do"
#define SCI_ENDWHILE		        L"end"

// Syntax to have for control structure
#define SCI_FOR			            L"for"
#define SCI_ENDFOR		            L"end"
#define SCI_BREAK		            L"break"
#define SCI_CONTINUE                L"continue"

// Syntax to have try/catch control structure
#define SCI_TRY			            L"try"
#define SCI_CATCH		            L"catch"
#define SCI_ENDTRY		            L"end"

// Syntax to have function declaration
#define SCI_FUNCTION		        L"function"
#define SCI_OPEN_RETURNS	        SCI_LBRACK
#define SCI_CLOSE_RETURNS	        SCI_RBRACK
#define SCI_OPEN_ARGS		        SCI_LPAREN
#define SCI_CLOSE_ARGS		        SCI_RPAREN
#define SCI_ENDFUNCTION		        L"endfunction"

// Syntax to have select control structure
#define SCI_SELECT		            L"select"
#define SCI_CASE		            L"case"
#define SCI_DEFAULT_CASE            L"else"
#define SCI_ENDSELECT	            L"end"

// Syntax to have return control structure
#define SCI_RETURN		            L"return"

// Syntax to describe implicit list
#define SCI_IMPLICIT_LIST	SCI_COLON

// Syntax to write Matrices
#define SCI_OPEN_MATRIX		        SCI_LBRACK
#define SCI_CLOSE_MATRIX	        SCI_RBRACK
#define SCI_LINE_SEPARATOR	        SCI_SEMICOLON
#define SCI_COLUMN_SEPARATOR        SCI_COMMA

// Syntax to write Cells
#define SCI_OPEN_CELL		        SCI_LBRACE
#define SCI_CLOSE_CELL		        SCI_RBRACE


/** \} */

#endif /* !__AST_TOKEN_HXX__ */
