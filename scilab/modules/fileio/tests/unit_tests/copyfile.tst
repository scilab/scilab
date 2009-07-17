// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// =============================================================================
// Date   : 13/12/2007
//
// Unitary tests for copyfile function
// =============================================================================
// TEST 1 : copyfile

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"프로그램",
"תוכנית"];


cd TMPDIR;
mkdir test_copyfile_source;
mkdir test_copyfile_target;
cd('test_copyfile_source');

for i = 1 : size(tab_ref,"*")
	filename = tab_ref(i) + '.orig';
	mputl('I am a dummy String : ' + tab_ref(i), filename);
	if fileinfo(filename) == [] then pause,end
end

for i = 1 : size(tab_ref,"*")
	orig = tab_ref(i) + '.orig';
	dest = tab_ref(i) + '.dest';
	r = copyfile(TMPDIR+'/test_copyfile_source/' + orig, TMPDIR+'/test_copyfile_target/' + dest);
	if r <> 1 then pause,end
	if fileinfo(TMPDIR+'/test_copyfile_target/' + dest) == [] then pause,end
end

cd TMPDIR;
rmdir('test_copyfile_source', 's');
rmdir('test_copyfile_target', 's');

// =============================================================================

cd TMPDIR;
dir1="test dir with space";
dir2="test dir with space number 2";
file1='dummyFile.dummy';
mkdir(dir1);
mkdir(dir2);

mputl('I am a dummy String', dir1+'/'+file1);
copyfile(dir1+'/'+file1,dir2);

lsResult = ls(dir2+'/'+file1);
if lsResult == [] then pause,end
