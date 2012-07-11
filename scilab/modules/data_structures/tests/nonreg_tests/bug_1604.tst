// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 1604 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1604
//
// <-- Short Description -->
//Bug in parser while manipulating tlist inside list.
//Note that the bug is only visible/reproducible in release mode under
//Windows. It should also happen in debug but can't be easily seen...


t=list(gca());//create the data
R = t(1).data_bounds(:,1); //crash
if R<> [0;1] then pause,end

