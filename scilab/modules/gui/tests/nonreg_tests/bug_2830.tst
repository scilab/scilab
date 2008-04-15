// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 2830 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2830
//
// <-- Short Description -->
//    crash clicking on show/hide toolbar (x64 build)

scf(0);
toolbar(0,'off');
toolbar(0,'on');