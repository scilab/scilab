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
// Unitary test for conversion of Matlab function: ISA

Syntaxes = [
    "%m2scideclare x|? ?|?|?"            ""
    "a = isa(x,''logical'')"             "a = or(type(x)==[4,6])"
    "b = isa(x,''char'')"                "b = type(x)==10"
    "c = isa(x,''numeric'')"             "c = or(type(x)==[1,5,8])"
    "C = isa(x,''integer'')"             "C = type(x)==8"
    "d = isa(x,''int8'')"                "d = typeof(x)==""int8"""
    "e = isa(x,''uint8'')"               "e = typeof(x)==""uint8"""
    "f = isa(x,''int16'')"               "f = typeof(x)==""int16"""
    "g = isa(x,''uint16'')"              "g = typeof(x)==""uint16"""
    "h = isa(x,''int32'')"               "h = typeof(x)==""int32"""
    "k = isa(x,''uint32'')"              "k = typeof(x)==""uint32"""
    "K = isa(x,''float'')"               "K = type(x)==1"
    "l = isa(x,''single'')"              "l = type(x)==1"
    "m = isa(x,''double'')"              "m = type(x)==1"
    "n = isa(x,''cell'')"                "n = typeof(x)==""ce"""
    "o = isa(x,''struct'')"              "o = typeof(x)==""st"""
    "p = isa(x,''function_handle'')"     "p = or(type(x)==[13,130])"
    "q = isa(x,''sparse'')"              "q = type(x)==5"
    "r = isa(x,''lti'')"                 "r = typeof(x) == ""state-space"""
    ""                                   "// !! L.20: Unhandled class: user_defined_class."
    "s = isa(x,''user_defined_class'')"  "s = isa(x,""user_defined_class"")"
    "%m2scideclare class|? ?|?|?"        ""
    "t = isa(x,class)"                   "t = mtlb_isa(x,class)"
    ];

Mtlb = Syntaxes(:,1);
Mtlb(Mtlb=="") = [];
mfile = TMPDIR + "/_isa";
mputl(Mtlb, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
res = mgetl(mfile + ".sci");

assert_checkequal(res, [""; Syntaxes(:,2)]);

