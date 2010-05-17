// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 3533 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3533
//
// <-- Short Description -->
//    lib(dir_path) doesn't work if the final slash is missing in dir_path.


//define some variables
function z = myplus(x, y), z = x + y,endfunction
function z = yourplus(x, y), x = x - y,endfunction
A=1:10;

//create the *.bin files in libdir
libdir=TMPDIR
save(libdir + '/myplus.bin', myplus);
save(libdir + '/yourplus.bin', yourplus);
save(libdir + '/A.bin', A);

//create the name file
mputl(['myplus';'yourplus';'A'],TMPDIR+'/names');

//build the library containing myplus and yourplus
ierr = execstr('mylib = lib(libdir)','errcatch');
if ierr <> 0 then pause,end

ierr = execstr('mylib = lib(libdir+filesep())','errcatch');
if ierr <> 0 then pause,end
