// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->


// <-- Non-regression test for bug 3102 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3102
//
// <-- Short Description -->
// regexp returns a "Unknown error"


r="/^b.*b.*b.*b.*b$/";
b = "b"; 
for i = 1:123
  b = b + "b";
end;
b = b + "r";
ierr=execstr("regexp(b, r)","errcatch");
if ierr <> 999 then pause,end

