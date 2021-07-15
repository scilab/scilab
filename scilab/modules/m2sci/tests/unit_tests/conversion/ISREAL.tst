// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// Unitary test for conversion of Matlab function: ISREAL

exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciTestConversion.sci", -1);

assert_checktrue(m2sciTestConversion("ISREAL"));

Syntaxes = [
    "isreal(true)"                   "isreal(%t*1)"
    "isreal(int8(-4))"               "isreal(int8(-4))"
    "isreal(1:3)"                    "isreal(1:3)"
    "isreal(1:.1:3)"                 "isreal(1:0.1:3)"
    "isreal(sparse([1 2]))"          "isreal(sparse([1,2]))"
    "isreal(sparse([1 2])<0.5)"      "isreal((sparse([1,2])<0.5)*1)"
    "isreal(''abcd'')"               "isreal(asciimat(""abcd""))"
    "isreal(gca())"                  "%t"   // questionnable: true in Matlab, not in Scilab
    "%m2scideclare a|2 3|Struct|?"   ""
    "isreal(a)"                      "%f"
    "%m2scideclare b|2 3|Cell|?"     ""
    "isreal(b)"                      "%f"
    "isreal(@sin)"                   "%f"
    "%m2scideclare d|1 2|?|?"        ""
    "isreal(d)"                      "isreal(d)"
    ];

mfile = TMPDIR + "/_isreal";
mputl(Syntaxes(:,1), mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
res = mgetl(mfile + ".sci");

assert_checkequal(res, [""; Syntaxes(:,2)]);
