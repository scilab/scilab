//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM MANDATORY -->

chdir(TMPDIR);

exec(SCI+"/modules/localization/tests/unit_tests/CreateDir.sce", -1);


tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"프로그램",
"תוכנית"];

for i = 1 : size(tab_ref,'*')
	sz = "dir_" + tab_ref(i);
	a = chdir(sz);
	if(a <> %T) then
		pause
	end
	b = pwd();
	if(b <> (TMPDIR + filesep() + sz)) then
		pause
	end
	cd(TMPDIR);
end
