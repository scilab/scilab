// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 9344 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9344
//
// <-- Short Description -->
// Parser did not display anything when failing on UTF-8 char

errmsg = ["mclose(1";"^";"Error: syntax error, unexpected end of line, expecting "","" or )"];
assert_checkerror("execstr(""mclose(1"")", errmsg);

errmsg = ["mclose(1°";"       ^~~^";"Error: syntax error, unexpected identifier, expecting end of file"];
assert_checkerror("execstr(""mclose(1°"")", errmsg);
