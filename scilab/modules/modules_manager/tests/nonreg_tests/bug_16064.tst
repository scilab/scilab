// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16064 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16064
//
// <-- Short Description -->
// tbx_make(Dir, "localization") = tbx_build_localization(Dir)
// never updated .mo files after changing .po files

dest = TMPDIR+"\bug_16064";
rmdir(dest, "s");
copyfile(SCI+"/contrib/toolbox_skeleton", dest);

tbx_make(dest, "localization");
addlocalizationdomain("toolbox_skeleton", dest+"\locales");
setlanguage fr;
assert_checkequal(_("toolbox_skeleton","Outline"), "Contour");

// Initial state of the fr_FR .mo file:
x0_fr = fileinfo(dest+"\locales\fr_FR\LC_MESSAGES\toolbox_skeleton.mo");
x0_en = fileinfo(dest+"\locales\en_US\LC_MESSAGES\toolbox_skeleton.mo");

// We change the fr_FR.po translated file:
txt = mgetl(dest+"\locales\fr_FR.po");
txt($) = "msgstr ""CONTOUR""";
mputl(txt, dest+"\locales\fr_FR.po");
sleep(2, "s")

// We rebuild the toolbox:
tbx_make(dest, "localization");

// The fr_FR .mo file must have been updated:
x1_fr = fileinfo(dest+"\locales\fr_FR\LC_MESSAGES\toolbox_skeleton.mo");
assert_checktrue(x1_fr(6) > x0_fr(6));
sleep(2, "s")
// But not the en_US one:
x1_en = fileinfo(dest+"\locales\en_US\LC_MESSAGES\toolbox_skeleton.mo");
assert_checktrue(x1_en(6) == x0_en(6));

//addlocalizationdomain("toolbox_skeleton", dest+"\locales");
//assert_checkequal(_("toolbox_skeleton","Outline"), "CONTOUR");

rmdir(dest,"s");
