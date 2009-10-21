// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// Create all needed directories
//==============================


chdir(TMPDIR);

if MSDOS then
	unix_w(jre_path()+"\bin\java.exe -cp "+SCI+"\modules\localization\tests\unit_tests CreateDir");
else
	unix_w(jre_path()+"/bin/java -classpath "+SCI+"/modules/localization/tests/unit_tests CreateDir");
end
