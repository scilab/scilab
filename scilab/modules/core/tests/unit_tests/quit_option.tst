// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2023 - Dassault Systèmes S.E. - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- NO CHECK ERROR OUTPUT -->

// <-- Short Description -->
// quit is an command line option
//

// Try to find the path of Scilab executable
//scilab path
if getos() == "Windows" then
    scilabBin = """" + WSCI + "\bin\scilex""";
else
    scilabBin = strsplit(SCI, "share/scilab")(1) + "/bin/scilab-cli";
end

exitcode = host(scilabBin + " -quit -e ""1+1"" --timeout 2m");
assert_checkequal(exitcode, 0);

filepath = fullfile(TMPDIR, "quit_option.sce");
code = "1 + 1";
mputl(code, filepath);

exitcode = host("< " + filepath + " " + scilabBin + " -quit --timeout 2m");
assert_checkequal(exitcode, 0);
// without -quit option the behavior is the same
exitcode = host("< " + filepath + " " + scilabBin + "  --timeout 2m");
assert_checkequal(exitcode, 0);

exitcode = host(scilabBin + " -quit -e ""error(''exit code will be set'')"" --timeout 2m");
assert_checktrue(exitcode <> 0 && exitcode <> 22 && exitcode <> 258);

code = "error(''exit code will be set'')";
mputl(code, filepath);

exitcode = host("< " + filepath + " " + scilabBin + " -quit --timeout 2m");
assert_checktrue(exitcode <> 0 && exitcode <> 22 && exitcode <> 258);
// without -quit option the behavior is the same
exitcode = host("< " + filepath + " " + scilabBin + "  --timeout 2m");
assert_checktrue(exitcode <> 0 && exitcode <> 22 && exitcode <> 258);
