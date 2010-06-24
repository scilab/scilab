// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 6951 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6951
//
// <-- Short Description -->
// stacksize had a strange behaviour.

// <-- JVM NOT MANDATORY -->

r = stacksize();
stacksize('max');
m1 = stacksize();
if r(1) >= m1(1) then pause, end

stacksize(r(1));
m2 = stacksize();
if r(1) <> m2(1) then pause, end

stack_test = 1e8;
stacksize(stack_test);
m3 = stacksize();
if m3(1) <> stack_test then pause, end
