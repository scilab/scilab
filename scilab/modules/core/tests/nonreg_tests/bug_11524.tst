// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 -INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11524 -->
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11524
//
// <-- Short Description -->
// vector ^ scalar operation tagged obsolete

//Here I can only test that the scalar^scalar operation and square^scalar are not tagged obsolete
2^2;
%s^2;
(1/%s)^2;
sparse(3)^2;

//the following tests display a warning that depends on the TMPDIR used by the test system
//the diary will not be constant so we skip it for automatic test
if %f then
  [1  2 3]^2;
  [1 %s]^3;
  [1 1/%s]^3;
  sparse([1 2 3])^2;
end
