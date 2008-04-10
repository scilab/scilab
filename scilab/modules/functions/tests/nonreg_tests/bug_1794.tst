// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1794 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1794
//
// <-- Short Description -->
//    Problem encountered when redefining builtin function

beta=5;

try
	beta
catch
	if %T then pause,end
end

function y=foo()
  beta;
  y=beta+1
endfunction

try
	foo()
catch
	if %T then pause,end
end
