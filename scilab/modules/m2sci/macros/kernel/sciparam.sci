// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
    "<>"      , "log" ;
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
