// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid BELAHCENE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1629 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1629
//
// <-- Short Description -->
//    Scila freezes when trying to convert the following m-file:
//    ----------------------------
//    function out=bug_test(par)
//    if (nargin<2)
//        par.a = 2;
//        par.p = 24;
//        par.v = 12;
//        par.m = 75;
//        par.n = 800;
//        par.q = 5;
//        par.r = 20;
//        par.i = 0.05;
//        par.z = 3.5;
//        par.l = -40;
//        par.d = 1000;
//        par.o = 8;
//        par.u = 0;
//    end
//    ---------------------
//    Note: without the nargin test surrounding the structure field assignments, the
// ...

MFILECONTENTS=["function out=bug1629(par)";
		"if (nargin<2)";
		"    par.a = 2;";
		"    par.p = 24;";
		"    par.v = 12;";
		"    par.m = 75;";
		"    par.n = 800;";
		"    par.q = 5;";
		"    par.r = 20;";
		"    par.i = 0.05;";
		"    par.z = 3.5;";
		"    par.l = -40;";
		"    par.d = 1000;";
		"    par.o = 8;";
		"    par.u = 0;";
		"end"]

MFILE=TMPDIR+"/bug1629.m"
SCIFILE=TMPDIR+"/bug1629.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR,%f,%t);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [out] = bug1629(par)";
		"";
		"// Output variables initialisation (not found in input variables)";
		"out=[];";
		"";
		"// Number of arguments in function call";
		"[%nargout,%nargin] = argn(0)";
		"";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"if %nargin<2 then";
		"  par.a = 2;";
		"  par.p = 24;";
		"  par.v = 12;";
		"  par.m = 75;";
		"  par.n = 800;";
		"  par.q = 5;";
		"  par.r = 20;";
		"  par.i = 0.05;";
		"  par.z = 3.5;";
		"  par.l = -40;";
		"  par.d = 1000;";
		"  par.o = 8;";
		"  par.u = 0;";
		"end;";
		"endfunction"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
