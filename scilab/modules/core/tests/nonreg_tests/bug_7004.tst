// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7004 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7004
//
// <-- Short Description -->
// Crash in siflibs after stack change of location (i.e. after a resize from gstacksize)

// <-- CLI SHELL MODE -->

gstacksize('max');
1+1;
