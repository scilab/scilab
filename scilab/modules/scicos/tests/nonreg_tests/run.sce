
// Disable vertical scrolling
lines(0);

// Load function LAUNCHTEST
exec("launchtest.sci");

// Set base directory for non-regression tests
if getversion() == "scilab-4.1.2" | getversion() == "Scilab-4.1.2-SVN"
	// TODO: do not depend on username and Scilab 5 path
	// WARNING: do not use "~" as a shortcut for home directory
	//          (otherwise copyfile fails later in LAUNCHTEST)
	baseDir = "/home/vaylet/dev/scilab-5.0/modules/scicos/tests/nonreg_tests";
elseif getversion() == "scilab-trunk-SVN" | getversion() == "scilab-5.0"
  baseDir = SCI + "/modules/scicos/tests/nonreg_tests";
end

// Test existence of base directory
[x,ierr] = fileinfo(baseDir);
if ierr ~= -1 & ~isempty(x)
	// Launch any available test (except skeleton.cos)
	cosFiles   = gsort(basename(listfiles("*.cos")),"lr","i");
	nbCosFiles = size(cosFiles,"*")
	if nbCosFiles == 0
		disp(msprintf(gettext("No test found in current directory: %s",pwd)))
	else
		for k = 1:nbCosFiles
			currentTestName = cosFiles(k);
			launchtest(baseDir,currentTestName);
		end
	end
else
	disp(msprintf("ERROR: base directory for tests does not exist: %s\n",baseDir))
end

exit

