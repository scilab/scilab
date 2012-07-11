// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4918 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4918
//
// message returned by 'get_absolute_file_path' is wrong

//$ touch /tmp/fun100.c
//$ LANG=C ./bin/scilab -nwni  -e 'get_absolute_file_path("/tmp/fun100.c")' -ns
// returned message must be :
// get_absolute_file_path: The file /tmp/fun100.c is not opened in scilab.
