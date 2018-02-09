// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14337 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- NOT FIXED --> 6.0.0
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14337
//
// <-- Short Description -->
// piping one command using a shell pipepine ( "|" ) works, but scilab segfaults at exit

if getos() == "Windows"
    cmd = "(1 + 1)";
    f = "echo " + cmd + " | " + WSCI + "\bin\scilab -nwni -ns > " + TMPDIR + "\out_bug_14337.txt";
    disp(f);
    ierr = host(f);
else
    cmd = """(1 + 1)""";
    ierr = host("echo " + cmd + " | nohup " + SCI + "/bin/scilab-cli -ns -quit > " + TMPDIR + "/out_bug_14337.txt 2>&1 &");
end

sleep(500); // wait for scilab to close
out_bug_14337_ref = mgetl(SCI + "/modules/core/tests/nonreg_tests/out_bug_14437.txt.ref");
out_bug_14337 = mgetl(TMPDIR + "/out_bug_14337.txt");

assert_checkequal(out_bug_14337, out_bug_14337_ref);
