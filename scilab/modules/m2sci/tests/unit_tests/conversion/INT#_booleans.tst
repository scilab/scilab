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
// Unitary test for conversion of Matlab functions INT# and UINT# on BOOLEANS

Syntaxes = [
    "% #() with booleans"
    "% --------------------"
    "#(true)             % on a constant"
    "#(5==3)             % on an boolean operation"
    "%m2scideclare a|2 5|Double|Real"
    "#(any(a))           % on a function call"
    "#(true(2,3))"
    "a = #(true(2,3))"
    "%m2scideclare b|2 5|Boolean|Boolean"
    "#(b)"
    "%m2scideclare c|2 5|Boolean|Unknown"
    "#(c)"
    "%m2scideclare d|2 5|Boolean|Sparse"
    "#(d)"
    "%m2scideclare e|? ?|Boolean|Boolean"
    "#(e)"
     ];
Ref = [
    ""
    "// #() with booleans"
    "// --------------------"
    "#(%t)  // on a constant"
    "#(5==3)  // on an boolean operation"
    ""
    "#(or(a,1))  // on a function call"
    "#(ones(2,3)==1)"
    "a = #(ones(2,3)==1)"
    ""
    "#(b)"
    ""
    "#(c)"
    ""
    "#(d)"
    ""
    "#(e)"
    ];

for fun = ["int8" "int16" "int32" "int64" "uint8" "uint16" "uint32" "uint64"]
    disp(fun)
    syntaxes = strsubst(Syntaxes, "#", fun);
    mfile = TMPDIR + "/"+fun;
    mputl(syntaxes, mfile+".m");
    mfile2sci(mfile+".m", TMPDIR)

    ref = strsubst(Ref, "#", fun);
    assert_checkequal(mgetl(mfile+".sci"), ref);
end
