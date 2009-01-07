// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 39065 -->
//

// <-- Short Description -->
//There is no way to know if the path returned by get_absolute_file_path() is the good one or not :
//if the input argument is not an opened file, the current directory is returned.
//=> It should return an error instead of just printing a message.

//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3906
//

// <-- INTERACTIVE TEST -->
// executes next line and checks that scilab not crashs

debug(4);exit

