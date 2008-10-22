// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Date   : 13/12/2007
//
// Unitary tests for copyfile function
// =============================================================================
// TEST 1 : copyfile
cd TMPDIR;
mkdir test_copyfile_source;
mkdir test_copyfile_target;

cd('test_copyfile_source');
mputl('I am a dummy String', 'dummyFile.dummy')
lsResult = ls("dummyFile.dummy");
if lsResult == [] then pause,end

r = copyfile(TMPDIR+'/test_copyfile_source/dummyFile.dummy',TMPDIR+'/test_copyfile_target/dummyFile.dummy');
if r <> 1 then pause,end

lsResult = ls(TMPDIR+'/test_copyfile_target/dummyFile.dummy');
if lsResult == [] then pause,end
mdelete(TMPDIR+'/test_copyfile_source/dummyFile.dummy');
mdelete(TMPDIR+'/test_copyfile_target/dummyFile.dummy');

rmdir(TMPDIR+'/test_copyfile_source');
rmdir(TMPDIR+'/test_copyfile_target');
// =============================================================================

cd TMPDIR;
dir1="test dir with space"
dir2="test dir with space number 2"
file1='dummyFile.dummy'
mkdir(dir1)
mkdir(dir2)

mputl('I am a dummy String', dir1+'/'+file1)
copyfile(dir1+'/'+file1,dir2)

lsResult = ls(dir2+'/'+file1);
if lsResult == [] then pause,end
