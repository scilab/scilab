// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005      - INRIA - Farid BELAHCENE
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1262 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1262
//
// <-- Short Description -->
//    Four errors of mfile2sci are reported
//    (scilab-3.0-u-20050309) Linux+Windows+Solaris.
//
//    Briefly: 
//    1) end; end -> endend
//    2) Initialization to empty matrix at wrong place
//    3) Problem with useless [ or ]
//    4) -(A-B) -> -A-B
//
//    Apply mfile2sci to the Matlab script given below.

MFILECONTENTS=["A = rand(3,3);"
	""
	"for ii=1:3; for jj=1:3; disp(A(ii,jj)); end; end"
	""
	"k=0;"
	"for ii=1:3;"
	"   for jj=1:3;"
	"      if k>2 then"
	"         B(ii,jj) = A(ii,jj);"
	"         k = 0;"
	"      else"
	"         k = k+1;"
	"      end;"
	"  end;"
	"end;"
	""
	"C = [A [A [1;2;3]]];"
	""
	"B = -(A-A)"];

MFILE=TMPDIR+"/bug1262.m";
SCIFILE=TMPDIR+"/bug1262.sci";

mputl(MFILECONTENTS,MFILE);

mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);


SCIFILECONTENTSREF=["";
	"// Display mode";
	"mode(0);";
	"";
	"// Display warning for floating point exception";
	"ieee(1);";
	"";
	"A = rand(3,3);"
	""
	"for ii = 1:3 for jj = 1:3 disp(A(ii,jj));end;end;"
	""
	"k = 0;"
	"for ii = 1:3"
	"  for jj = 1:3"
	"    if k>2 then"
	"      B(ii,jj) = A(ii,jj);"
	"      k = 0;"
	"    else"
	"      k = k+1;"
	"    end;"
	"  end;"
	"end;"
	""
	"C = [A,A,[1;2;3]];"
	""
	"B = -(A-A)"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
