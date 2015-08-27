//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4065 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4065
//
// <-- Short Description -->
//   When filter is used with polynomials as arguments num and den, the degree 0 monomial is ignored.
Num=1;Den=1+%z;u=[1,2,3,4,5];
if or(filter(Num,Den,u)<>filter([0 1],[1 1],u)) then pause,end
if or(filter(%z,Den,u)<>filter(1,[1 1],u)) then pause,end

