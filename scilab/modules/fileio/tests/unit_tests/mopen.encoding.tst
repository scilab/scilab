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
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"תוכנית"];

lang_name = ["Simplified Chinese",
        "Cyrillic",
        "Japanese",
        "Thai",
        "Arabish",
        "Vietanmien",
        "Hebreu"];

for i = 1 : size(tab_ref,'*')
	dz = "dir_" + tab_ref(i);
	p = cd(dz);
	
	fd1 = mopen('file_'+tab_ref(i),'rt');
	r1 = mgetl(fd1);
	mclose(fd1);
	
	ref_str = 'str_' + tab_ref(i) + ' : ' + lang_name(i);
	if (r1 <> ref_str) then pause,end
	
	cd(TMPDIR);
	
	filenam = TMPDIR + filesep() + "dir_" + tab_ref(i) + filesep() + 'file_'+tab_ref(i);
	fd2 = mopen(filenam);
	r2 = mgetl(fd2);
	mclose(fd2);
	
	if (r2 <> ref_str) then pause,end
	
end
