// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9749 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9749
//
// <-- Short Description -->
// expm reports non convergence problem

A = [-1174.241,449.21242,5998.315;-615.9422,-946.33405,7421.3183;0,0,0];
s=spec(A);
for k=0:2:32
  if execstr("E=expm(A*2^k)","errcatch")<>0 then pause,end;
  S=spec(E);
  assert_checkalmostequal (S , exp(s*2^k),1d-12,1d-12);
end
