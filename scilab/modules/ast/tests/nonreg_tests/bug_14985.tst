// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI Group - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14985 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14985
//
// <-- Short Description -->
// scilab crashes if a .bin file was not found

//if file is deleted
cd TMPDIR;
mkdir buglib;
mputl(["function test(a,b)";"disp([a,b])";"endfunction"],"buglib/test.sci");
genlib("blib","buglib");
blib=lib("buglib");
test(1,2) //ok
deletefile("buglib/test.bin");
clear;
blib=lib("buglib");

msg = msprintf(_("Unable to open : %s.\n"), fullfile(TMPDIR, "buglib", "test.bin"));
assert_checkerror("test(1, 2)", msg); //avoid crash

//or if the file is not reachable ( trouble with éàè in path of .bin files )
clear;
path = "boite à outils";
cd TMPDIR;
mkdir(path);
mputl(["function test(a,b)";"disp([a,b])";"endfunction"], fullfile(path, "test.sci"));
genlib("blib", path);
blib=lib(path);
test(1,2) //ok
