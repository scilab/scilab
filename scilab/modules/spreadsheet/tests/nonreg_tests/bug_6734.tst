// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6734 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6734
//
// <-- Short Description -->
//  
// read_csv function contains erroneous error message.

ierr = execstr("M = read_csv(''blaat.txt'')", "errcatch");
if ierr <> 10000 then pause,end
L = lasterror();
if grep(L(1),"read_csv") == [] then pause,end

