//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4282 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4282
//
// <-- Short Description -->
// If scilab is launched from a symbolic link, the current directory (in scilab) is always "SCI" instead of the
// path from which scilab is launched
//

// <-- INTERACTIVE TEST -->
// ln -s <SCI>/bin/scilab /tmp/scilab
// cd /tmp
// ./scilab -nwni -nb
// pwd() should return "/tmp"
