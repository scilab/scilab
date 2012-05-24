// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- LONG TIME EXECUTION -->
//
// <-- Non-regression test for bug 8563 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8563
//
// <-- Short Description -->
//  Scilab under GNU/Linux 64 bit was unable to load files bigger than 2GB.
//

// On a 64 bit platform, generate or use a big file (>= 2 Gb)
// replaces BIG_FILE_FILENAME by this filename

BIG_FILE_FILENAME = "";
fd = mopen(BIG_FILE_FILENAME, "rb");
if execstr("mseek(2000000000, fd);", "errcatch") <> 0 then pause, end

