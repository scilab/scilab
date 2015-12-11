//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM MANDATORY -->

cd(TMPDIR);

exec(SCI+"/modules/localization/tests/unit_tests/CreateDir.sce", -1);


tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"حريات وحقوق",
"תוכנית"];

for i = 1 : size(tab_ref,'*')
	fz = TMPDIR + filesep() + "dir_" + tab_ref(i) + filesep() + 'file_' + tab_ref(i) +'.txt';
	fd = mopen(fz,'wt'); mclose(fd);
	if isfile(fz) <> %t then pause,end
end
// =============================================================================
f = [SCI+'/etc/scilab.start';SCI+'/etc/scilab.quit';SCI+'/Wrong_file_or_path'];
ref = [%T;%T;%F];
if and(isfile(f) <> ref) then pause,end
// =============================================================================
f = SCI+ '/modules/';
ref = %f;
if and(isfile(f) <> ref) then pause,end
// =============================================================================
ierr = execstr('isfile()','errcatch');
if ierr <> 77 then pause,end
// =============================================================================
ierr = execstr('isfile(1)','errcatch');
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr('isfile(%f)','errcatch');
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr('isfile(''file1'',''file2'')','errcatch');
if ierr <> 77 then pause,end
// =============================================================================
