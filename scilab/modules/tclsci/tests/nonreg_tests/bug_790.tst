// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre .marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 790 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=790
//
// <-- Short Description -->
//    Scilab crashes when you enter findobj().
//    I know that it is not usefull but, the bug exists.

refMsg = msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "findobj", 1, 2);
assert_checkerror("findobj()", refMsg)
assert_checktrue(execstr("findobj(""test"") == []","errcatch")==0);
assert_checktrue(execstr("findobj(""Param1"",""Param2"") == []","errcatch")==0);
