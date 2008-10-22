// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2284 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2284
//
// <-- Short Description -->
//    Numerical error when carrying out a particular calculation using the
//    complex exp function.  E.g.
//
//    -->20 * exp(%i * 2 * %pi / 3)
//     ans  =
//
//      -11. + 17.320508i                             
//
//    This is the wrong result!!

s1 = -9.999999999;
s2 = 0.999999999999;

print(TMPDIR+'/bug2284',s1,s2);
txt = mgetl(TMPDIR+'/bug2284');
if or(txt <> [" s2  =";" ";"    1.  ";" s1  =";" ";"  -10.  "]) then pause,end
mdelete(TMPDIR+'/bug2284');
