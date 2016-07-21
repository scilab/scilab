//<-- CLI SHELL MODE -->
//<-- ENGLISH IMPOSED -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

tab_ref = [ ..
"世界您好", ..
"азеазея", ..
"ハロー・ワールド", ..
"เฮลโลเวิลด์", ..
"حريات وحقوق", ..
"프로그램", ..
"프로그램", ..
"תוכנית"];

cd TMPDIR;
mkdir open_xls_tests;
cd open_xls_tests;
origfile = SCI + "/modules/spreadsheet/tests/unit_tests/readxls.xls";

for i = 1 : size(tab_ref, "*")
	newfile = tab_ref(i) + ".xls";
	copyfile(origfile, newfile);
	[fileID, fileStream, SheetNames, SheetPos] = xls_open(newfile);
	if(fileID == 0) then pause, end
	mclose(fileID);
end

cd TMPDIR;
removedir open_xls_tests;

instr = "[fd, SST, Sheetnames, Sheetpos] = xls_open();";
refMsg = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "xls_open", 1);
assert_checkerror(instr, refMsg);
