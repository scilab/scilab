// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 883 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=883
//
// <-- Short Description -->
//    format() output oddities

SCIFILE = TMPDIR+"/bug883.dia";
diary(SCIFILE);

a = %pi/100;
b = 2*%pi/a;
for i=2:25
 format(i);
 disp(b);
end;
diary(0);

SCIFILECONTENTS=mgetl(SCIFILE);

REFCONTENTS=[" ";
		"   **  ";
		" ";
		"   ***  ";
		" ";
		"   200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    200.  ";
		" ";
		"    199.99999999999997  ";
		" ";
		"    199.999999999999972  ";
		" ";
		"    199.9999999999999716  ";
		" ";
		"    199.99999999999997158  ";
		" ";
		"    199.999999999999971578  ";
		" ";
		"    199.9999999999999715783  ";
		" ";
		"    199.99999999999997157829  "];
if or(REFCONTENTS<>SCIFILECONTENTS) then pause,end
