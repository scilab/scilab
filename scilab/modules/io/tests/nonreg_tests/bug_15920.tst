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
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15920
//
// genlib() does not regenerate a missing .bin if the .sci is unchanged (Scilab 6 regression)

path = fullfile(TMPDIR,"test");
rmdir(path, 's');
mkdir(path);
mputl("function a=pi(), a = %pi; endfunction", fullfile(path,"pi.sci"));
genlib("pilib", path)
deletefile(fullfile(path,"pi.bin"))
clear pilib
genlib("pilib", path)
assert_checktrue(isfile(fullfile(path,"pi.bin")))
