function sciparam()
// Copyright INRIA
// Scilab Projet - V. Couvert
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
