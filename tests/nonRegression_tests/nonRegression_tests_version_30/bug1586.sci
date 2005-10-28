// Non-regression test file for bug 1586
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 octobre 2005

mode(-1);
clear;

dirToSave = pwd();

cd(TMPDIR);
[status,msg]=mkdir('bug1586');
[status,msg]=mkdir('bug1586');

if( status == 2 )
	affich_result(%T,1586);
else
	affich_result(%F,1586);
end

rmdir('bug1586');
cd(dirToSave);

clear
