// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function sciparam()
// M2SCI variable declaration
// All variables listed below can be accessed for reading from all macros called from m2sci() macro

quote="''"
logics=["==","<",">","<=",">=","<>"]

// Operations code
// [ operation symbol, operation translation function code ; ...]
// e.g. : translation function for addition (+) is named %a2sci()...
ops = [ "+"       , "a"   ;
	"-"       , "s"   ;
	"*"       , "m"   ;
	".*"      , "x"   ;
	"*."      , "u"   ;
	".*."     , "k"   ;
	"/"       , "r"   ;
	"./"      , "d"   ;
	"/."      , "v"   ;
	"./."     , "y"   ;
	"\"       , "l"   ;
	".\"      , "q"   ;
	"\."      , "w"   ;
	".\."     , "z"   ;
	"^"       , "p"   ;
	"=="      , "log" ;
	"<"       , "log" ;
	">"       , "log" ;
	"<="      , "log" ;
	">="      , "log" ;
	"~="      , "log" ;
	":"       , "imp" ;
	"rc"      , "rc"  ;
	"ins"     , "i"   ;
	"ext"     , "e"   ;
	quote     , "t"   ;
	"cc"      , "cc"  ;
	"|"       , "g"   ; 
	"&"       , "h"   ;
	"~"       , "5"   ;
	".^"      , "j"   ;
	"."+quote , "0"   ;
	"cceol"      , "cc"  ] // Column Concatenation with EOL between Rows

    [logics,ops,quote]=resume(logics,ops,quote)

endfunction
