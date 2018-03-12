// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15472 -->
//
// http://bugzilla.scilab.org/show_bug.cgi?id=15472
//
// <-- Short Description -->
// mdelete("test") worked like mdelete("test.*") under Windows 
// =============================================================================
// bug is in "listfiles" function and not directly mdelete.

cd(TMPDIR);
mputl("", "test");
mputl("", "test2");
mputl("", "test.sci");
assert_checkequal(isfile(["test" "test2" "test.sci"]), [%t %t %t]);
x = listfiles("test");
assert_checkequal(x, "test");

createdir("dir1");
mputl("", "dir1/test");
createdir("dir2");
mputl("", "dir2/test");
createdir("dir3");
mputl("", "dir3/test");

ref = [ fullfile("dir1", "test")
        fullfile("dir2", "test")
        fullfile("dir2", "test")]
    
x = listfiles(ref);
assert_checkequal(x, ref);
