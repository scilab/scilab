//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4620 -->
//

// <-- Short Description -->
// the setenv function makes scilab hangs
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4620
//

A = 1:100000;
B = strcat(string(A),"");

ierr = execstr("r = setenv(''TEST_FOO'',B);","errcatch");
if ierr <> 0 then pause,end

//SetEnvironmentVariable ( setenv on windows ) is able to set variable > 32767 ( _MAX_ENV )
if getos() <> "Windows" then
    if r <> %F then pause,end
    if getenv("TEST_FOO","") <> "" then pause,end
else
    if r <> %T then pause,end
    if getenv("TEST_FOO","") <> B then pause,end
end

