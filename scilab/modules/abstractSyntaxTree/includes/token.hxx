/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __AST_TOKEN_HXX__
#define __AST_TOKEN_HXX__

/** Scilab tokens and key words
 ** \{ */

#define SCI_DOT			"."

#define SCI_TRUE		"%T"
#define SCI_FALSE		"%F"

#define SCI_INT8		"int8"
#define SCI_INT16		"int16"
#define SCI_INT32		"int32"
#define SCI_INT64		"int64"
#define SCI_LPAREN		"("
#define SCI_RPAREN		")"
#define SCI_LBRACE		"{"
#define SCI_RBRACE		"}"
#define SCI_LBRACK		"["
#define SCI_RBRACK		"]"

#define SCI_PLUS		"+"
#define SCI_MINUS		"-"
#define SCI_TIMES		"*"
#define SCI_DIVIDE		"/"
#define SCI_RDIVIDE		"\\"
#define SCI_POWER		"^"

#define SCI_DOTTIMES		".*"
#define SCI_DOTDIVIDE		"./"
#define SCI_DOTRDIVIDE		".\\"
#define SCI_DOTPOWER		".^"

#define SCI_KRONTIMES		".*."
#define SCI_KRONDIVIDE		"./."
#define SCI_KRONRDIVIDE		".\\."

#define SCI_CONTROLDIVIDE	"/."

#define SCI_EQ			"=="
#define SCI_NE			"<>"
#define SCI_GE			">="
#define SCI_GT			">"
#define SCI_LE			"<="
#define SCI_LT			"<"

#define SCI_AND			"&"
#define SCI_OR			"|"
#define SCI_ANDAND	"&&"
#define SCI_OROR		"||"
#define SCI_NOT			"~"

#define SCI_CONJUGATE_TRANSPOSE	"'"
#define SCI_TRANSPOSE		".'"

#define SCI_QUOTE		"\'"
#define SCI_DQUOTE		"\""
#define SCI_COMMA		","
#define SCI_SEMICOLON		";"
#define SCI_COLON		":"

#define SCI_DOLLAR		"$"

#define SCI_ASSIGN		"="
#define SCI_OPEN_COMMENT	"//"

// Syntax to acces field 'bar' in variable 'foo'
// foo.var
#define SCI_FVAR_SEPARATOR	SCI_DOT

// Syntax to access indexed variable in 'foo'
// foo(5)
#define SCI_SVAR_OPEN		SCI_LPAREN
#define SCI_SVAR_CLOSE		SCI_RPAREN

// Syntax to do strings
#define SCI_OPEN_STRING		SCI_DQUOTE
#define SCI_CLOSE_STRING	SCI_DQUOTE

// Syntax to have function calls
#define SCI_OPEN_CALL		SCI_LPAREN
#define SCI_CLOSE_CALL		SCI_RPAREN
#define SCI_ARGS_SEPARATOR	SCI_COMMA

// Syntax to have if control structure
#define SCI_IF			"if"
#define SCI_THEN		"then"
#define SCI_ELSE		"else"
#define SCI_ENDIF		"end"
#define SCI_OPEN_TEST		SCI_LPAREN
#define SCI_CLOSE_TEST		SCI_RPAREN

// Syntax to have while control structure
#define SCI_WHILE		"while"
#define SCI_DO			"do"
#define SCI_ENDWHILE		"end"

// Syntax to have for control structure
#define SCI_FOR			"for"
#define SCI_ENDFOR		"end"
#define SCI_BREAK		"break"

// Syntax to have try/catch control structure
#define SCI_TRY			"try"
#define SCI_CATCH		"catch"
#define SCI_ENDTRY		"end"

// Syntax to have function declaration
#define SCI_FUNCTION		"function"
#define SCI_OPEN_RETURNS	SCI_LBRACK
#define SCI_CLOSE_RETURNS	SCI_RBRACK
#define SCI_OPEN_ARGS		SCI_LPAREN
#define SCI_CLOSE_ARGS		SCI_RPAREN
#define SCI_ENDFUNCTION		"endfunction"

// Syntax to have select control structure
#define SCI_SELECT		"select"
#define SCI_CASE		"case"
#define SCI_ENDSELECT		"end"

// Syntax to have return control structure
#define SCI_RETURN		"return"

// Syntax to describe implicit list
#define SCI_IMPLICIT_LIST	SCI_COLON

// Syntax to write Matrixes
#define SCI_OPEN_MATRIX		SCI_LBRACK
#define SCI_CLOSE_MATRIX	SCI_RBRACK
#define SCI_LINE_SEPARATOR	SCI_SEMICOLON
#define SCI_COLUMN_SEPARATOR	SCI_COMMA

// Syntax to write Cells
#define SCI_OPEN_CELL		SCI_LBRACE
#define SCI_CLOSE_CELL		SCI_RBRACE


/** \} */

#endif /* !__AST_TOKEN_HXX__ */
