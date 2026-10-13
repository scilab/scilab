// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre .marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- Non-regression test for bug 790 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/790
//
// <-- Short Description -->
//    Scilab crashes when you enter findobj().
//    I know that it is not usefull but, the bug exists.

assert_checktrue(findobj() == get(0));
assert_checktrue(findobj("test") == []);
assert_checktrue(findobj("Param1", "Param2") == []);
