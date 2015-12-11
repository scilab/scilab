// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3314 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3314
//
// <-- Short Description -->

// linspace(0,1,(1-0.9)*22050) crashes. (Scilab 4.1.2 on linux Ubuntu 8.04)

// Test updated for Scilab 5.5.0: using non-integer values for third input argument now returns an error.

ierr = execstr("linspace(0,1,(1-0.9)*22050);","errcatch");
if ierr == 0 then pause,end

a = (1-0.9)*22050;
ierr = execstr("linspace(0,1,a);","errcatch");
if ierr == 0 then pause,end

