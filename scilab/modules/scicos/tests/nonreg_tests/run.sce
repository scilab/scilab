// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Disable vertical scrolling
lines(0);

// Set base directory for non-regression tests
if getversion() == "scilab-4.1.2" | getversion() == "Scilab-4.1.2-SVN"
	// TODO:    do not depend on username and Scilab 5 path
	// WARNING: do not use "~" as a shortcut for home directory
	//          (otherwise copyfile fails later in LAUNCHTEST)
	baseDir = "/home/vaylet/dev/scilab-5.0/modules/scicos/tests/nonreg_tests";
elseif getversion() == "scilab-trunk-SVN" | getversion() == "scilab-5.0"
  baseDir = SCI + "/modules/scicos/tests/nonreg_tests";
end

// Test existence of base directory
[x,ierr] = fileinfo(baseDir);
if ierr ~= -1 & ~isempty(x)
	// Launch any available test
	cosFiles   = gsort(basename(listfiles("*.cos")),"lr","i");
	nbCosFiles = size(cosFiles,"*")
	if nbCosFiles == 0
		error(mprintf(gettext("%s: No test found in current directory: %s"), "run.sce", pwd))
	else
		for k = 1:nbCosFiles
			currentTestName = cosFiles(k);
			launchtest(baseDir,currentTestName);
		end
	end
else
	error(mprintf(gettext("%s: Base directory for tests does not exist: %s\n"), "run.sce", baseDir))
end

exit

