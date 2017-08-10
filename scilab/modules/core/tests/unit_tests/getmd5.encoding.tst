//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

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

v_ref = [
'67737ef45b718d9e151929715a2a9557',
'0e68dfdd1701d509731099cb245b05f0',
'3451966ed3a31bf8ebc5ff621e54ed46',
'090d128546828182ff488b8dcf24792c',
'cc4031dc819e21b7428480ddcf8761d3',
'c654f8795440063a2bed82518065ecf0',
'd4c784f6566873efb0bb25c858b28fc8'];

for i = 1 : size(tab_ref,'*')
	fz = TMPDIR + filesep() + "dir_" + tab_ref(i) + filesep() + "file_" + tab_ref(i);
	if getmd5(fz) <> v_ref(i) then pause,end
end
cd(TMPDIR); 
