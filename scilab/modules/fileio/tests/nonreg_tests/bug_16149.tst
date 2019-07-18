// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16149 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=16149
//
// <-- Short Description -->
// fullpath does not support symbolic links

cd(TMPDIR)
mkdir("testdir")
if getos() <> "Windows" then
    unix_w("ln -sf "+TMPDIR+"/testdir tmp");
else
    host("mklink /D tmp "+TMPDIR+"\testdir");
end

// symbolic link
assert_checkequal(fullpath("tmp/hello.txt"),strcat([TMPDIR "testdir" "hello.txt"], filesep()));

// symbolic link (recursive)
assert_checkequal(fullpath("tmp/../tmp/hello.txt"),strcat([TMPDIR "testdir" "hello.txt"], filesep()));

// delete the symbolic link
if getos() <> "Windows" then
    deletefile("tmp");
else
    host("rmdir tmp");
end

// raw path when tmp is not a symbolic link
assert_checkequal(fullpath("tmp/hello.txt"),strcat([TMPDIR "tmp" "hello.txt"], filesep()));
assert_checkequal(fullpath("tmp/../tmp/hello.txt"),strcat([TMPDIR "tmp" "hello.txt"], filesep()));
assert_checkequal(fullpath("tmp/tmp/../../hello.txt"),strcat([TMPDIR "hello.txt"], filesep()));

