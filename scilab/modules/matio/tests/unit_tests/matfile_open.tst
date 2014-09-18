// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

filename=TMPDIR+"/test_matfile.mat";

A = rand(10,10);
B = sprand(100,100,0.1);
savematfile(filename,'A','B','-v6');
clear();
filename=TMPDIR+"/test_matfile.mat";
fd = matfile_open(filename);
assert_checkequal(fd, 0);
matfile_close(fd);
assert_checktrue(isfile(filename));
