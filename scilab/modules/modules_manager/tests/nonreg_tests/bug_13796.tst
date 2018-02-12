// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 13796 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13796
//
// <-- Short Description -->
// tbx_generate_pofile()
//  - missed _() and _gettext() references.
//  - yielded #: comments with absolute location paths instead of relative
//    to the module root
//  - wrapped long lines even when they should not be so

path = TMPDIR+"/bug_13796";
copyfile(SCI+"/contrib/toolbox_skeleton", path);
mdelete(path+"/locales/*");
tbx_generate_pofile(path);

// Checking existence of the po file:
assert_checktrue(isfile(path+"/locales/en_US.po"));

// Checking some content:
f = mgetl(path+"/locales/en_US.po");
assert_checktrue(grep(f, pathconvert("#: ~\sci_gateway\cpp\sci_cpperror.cpp:23",%f))~=[]);
assert_checktrue(grep(f, "msgid ""%s: I''m waiting for only one argument.\n""")~=[]);
assert_checktrue(grep(f, "#: a XML file:5")~=[]);
assert_checktrue(grep(f, "msgid ""Height""")~=[]);

// Cleaning
rmdir(path,"s");
