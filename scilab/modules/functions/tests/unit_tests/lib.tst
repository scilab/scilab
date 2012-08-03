// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

warning("off");

cd(TMPDIR);
mkdir('test_lib');
cd('test_lib');

//define some variables
function z = myplus(x, y), z = x + y,endfunction
function z = yourplus(x, y), x = x - y,endfunction
A=1:10;

//create the *.bin files in libdir
libdir = TMPDIR + filesep() + 'test_lib';
save(libdir + '/myplus.bin', myplus);
save(libdir + '/yourplus.bin', yourplus);
save(libdir + '/A.bin', A);

//create the name file
mputl(['myplus';'yourplus';'A'],libdir+'/names');

//erase the variables
clear myplus yourplus A

//build the library containing myplus and yourplus
test_lib = lib(libdir+'/');

ref = 1:10;
if (A <> ref) then pause,end

if isdef('myplus') <> %t then pause,end
if isdef('yourplus') <> %t then pause,end
if type(yourplus) <> 13 then pause,end
if type(myplus) <> 13 then pause,end
if type(test_lib) <> 14 then pause,end


