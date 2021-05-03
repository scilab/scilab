// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// Unitary test for conversion of Matlab CONCATENATIONS

// --------------------------------------------------------------------
// Octave rules:
// ------------
// ** concatenating integers of various inttypes is possible.
//    The first integer operand sets the inttype of the result.
// * [Boolean|Real, Int]   =>  Int (1st inttype wins)
// * [Complex|Sparse, Int] => error
// * [Double, Sparse] => Sparse
// * [Char, Boolean|Int|Real|Complex|Sparse] => Char, with numbers = ASCII
// **  concatenation of [dense, sparse] => sparse, as in Scilab

Syntaxes = [
    "% [ , ]"                                                  "// [ , ]"
    "% ====="                                                  "// ====="
    "a = [true, sparse([2 3])]"                                "a = [%t,sparse([2,3])]"
    "%m2scideclare a|1 3|Sparse|Real"                          ""
    "a = [pi, sparse([2 3])]"                                  "a = [%pi,sparse([2,3])]"
    "%m2scideclare a|1 3|Sparse|Real"                          ""
    "a = [i, sparse([2 3])]"                                   "a = [%i,sparse([2,3])]"
    "%m2scideclare a|1 3|Sparse|Complex"                       ""
    "a = [true i 2 sparse([2 3])]"                             "a = [%t,%i,2,sparse([2,3])]"
    "%m2scideclare a|1 5|Sparse|Complex"                       ""
    ""                                                         ""
    "% With INTEGERS:"                                         "// With INTEGERS:"
    "a = [true int8(-6)]"                                      "a = [%t,int8(-6)]"
    "%m2scideclare a|1 2|Int|int8"                             ""
    "a = [true, uint16(3), int8(-6)]"                          "a = [%t,uint16(3),int8(-6)]"
    "%m2scideclare a|1 3|Int|uint16"                           ""
    "%a = [i, int8(-5)]          % Octave => Error"            "//a = [i, int8(-5)]          % Octave => Error"
    "%a = [sparse(3), int8(-5)]  % Octave => Error"            "//a = [sparse(3), int8(-5)]  % Octave => Error"
    ""                                                         ""
    "% With STRINGs :"                                         "// With STRINGs :"
    "a = [[''AB'';''CD''],[''R'';''T''], [''U'';''V'']]"       "a = [""AB"";""CD""] + [""R"";""T""] + [""U"";""V""]"
    "%m2scideclare a|2 4|String|Char"                          ""
    ""                                                         ""
    "% [ ; ]"                                                  "// [ ; ]"
    "% ====="                                                  "// ====="
    "a = [true ; sparse([2 ; 3])]"                             "a = [%t;sparse([2;3])]"
    "%m2scideclare a|3 1|Sparse|Real"                          ""
    "a = [sparse([2 ; 3]) ; true]"                             "a = [sparse([2;3]);%t]"
    "%m2scideclare a|3 1|Sparse|Real"                          ""
    "a = [pi ; sparse([2 ; 3])]"                               "a = [%pi;sparse([2;3])]"
    "%m2scideclare a|3 1|Sparse|Real"                          ""
    "a = [sparse([2 ; 3]) ; pi]"                               "a = [sparse([2;3]);%pi]"
    "%m2scideclare a|3 1|Sparse|Real"                          ""
    "a = [i ; sparse([2 ; 3])]"                                "a = [%i;sparse([2;3])]"
    "%m2scideclare a|3 1|Sparse|Complex"                       ""
    "a = [sparse([2 ; 3]) ; i]"                                "a = [sparse([2;3]);%i]"
    "%m2scideclare a|3 1|Sparse|Complex"                       ""
    "a = [true ; i ; 2 ; sparse([2 ; 3])]"                     "a = [%t;%i;2;sparse([2;3])]"
    "%m2scideclare a|5 1|Sparse|Complex"                       ""
    "a = [2 ; i ; sparse([2 ; 3]) ; true]"                     "a = [2;%i;sparse([2;3]);%t]"
    "%m2scideclare a|5 1|Sparse|Complex"                       ""
    ""                                                         ""
    "% With INTEGERS:"                                         "// With INTEGERS:"
    "a = [true ; int8(-6)]"                                    "a = [%t;int8(-6)]"
    "%m2scideclare a|2 1|Int|int8"                             ""
    "a = [true ; uint16(3) ; int8(-6)]"                        "a = [%t;uint16(3);int8(-6)]"
    "%m2scideclare a|3 1|Int|uint16"                           ""
    "%a = [i ; int8(-5)]          % Octave => Error"           "//a = [i ; int8(-5)]          % Octave => Error"
    "%a = [sparse(3) ; int8(-5)]  % Octave => Error"           "//a = [sparse(3) ; int8(-5)]  % Octave => Error"
    ""                                                         ""
    "% With STRINGs :"                                         "// With STRINGs :"
    "a = [[''AB'', ''C''] ; [''RS'', ''T''] ; [''UV'',''W'']]" "a = [""AB""+""C"";""RS""+""T"";""UV""+""W""]"
    "%m2scideclare a|3 3|String|Char"                          ""
    ];

mfile = TMPDIR + "/_concatenations";
mputl(Syntaxes(:,1), mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");
Ref = ["" ; Syntaxes(1:$-1,2)];

assert_checkequal(Res, Ref);

