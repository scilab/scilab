// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- NO CHECK ERROR OUTPUT -->  // to avoid warning from the doc builder
// <-- TEST WITH GRAPHIC -->      // needed to create images in the doc

// <-- Non-regression test for bug 15423 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15423
//
// <-- Short Description -->
// tbx_make(toolbox_skeleton, section) executed the existing builder instead
//  of only selected sections.
// After fixing this,
// tbx_make(toolbox_skeleton, "help")  yielded an error Undefined variable: TOOLBOX_TITLE
// tbx_make(toolbox_skeleton, "macros") yielded an error Undefined variable: TOOLBOX_NAME

path = TMPDIR+filesep()+"bug_15423";
copyfile(SCI+"/contrib/toolbox_skeleton", path);

assert_checktrue(execstr("tbx_make(path, ""help"")", "errcatch")==0);
assert_checktrue(execstr("tbx_make(path, ""macros"")", "errcatch")==0);

// Cleaning
rmdir(path,"s");
