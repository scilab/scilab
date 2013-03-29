// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 12083 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12083
//
// <-- Short Description -->
// Load multiple variables could failed due to missing "clear" of previous variables.

plot3d();
f=gcf();
path = fullpath(TMPDIR + "/../");
xs2bmp(f, path + "original.bmp");
original_md5 = getmd5(path + "original.bmp");
deletefile(path + "original.bmp");

//save f, gcf, %_plot3d, original_md5 ...
save(path + "bug_12083.sod");

//close Scilab and retart it
path = fullpath(TMPDIR + "/../");
load(path + "bug_12083.sod");
deletefile(path + "bug_12083.sod");
f=gcf();
xs2bmp(f, path + "restored.bmp");
restored_md5 = getmd5(path + "restored.bmp");
deletefile(path + "restored.bmp");

assert_checkequal(original_md5, restored_md5);
