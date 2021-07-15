// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
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
// Unitary test for conversion of Matlab function: FULL

exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciTestConversion.sci", -1);
assert_checktrue(m2sciTestConversion("FULL"));

// ---------------------------------------------

Syntaxes = [
    "full(sparse([true false]))"
    "full([true false])"
    "full(int8([-8 5]))"
    "full([-8 5])"
    "full(1:0.1:3)"
    "full(sparse([-8 5]))"
    "full([''ABC'' ; ''DEF''])"
    "%full(@sin) % error in Scilab as in Octave"
    ];
Ref = [
  "full(sparse([%t,%f]))"
  "full([%t,%f])"
  "full(mtlb_int8([-8,5]))"
  "full([-8,5])"
  "full(1:0.1:3)"
  "full(sparse([-8,5]))"
  "full([""ABC"";""DEF""])"
  "//full(£sin) % error in Scilab as in Octave"
    ];
Mtlb = Syntaxes(:,1);
Mtlb(Mtlb=="") = [];
mfile = TMPDIR + "/_full";
mputl(Mtlb, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
res = mgetl(mfile + ".sci");

assert_checkequal(res, [""; Ref]);

