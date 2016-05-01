//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3478 -->
//

// <-- Short Description -->
// file returns a wrong error message when file does not exists.
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3478
//

ierr = execstr("file(''open'', ''my file which does not exist'', ''old'');", "errcatch");
if ierr <> 240 then pause,end
