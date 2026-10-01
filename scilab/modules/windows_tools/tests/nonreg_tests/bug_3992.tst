// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3992 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/3992
//
// <-- Short Description -->
// fix some invalid dependencies on external libraries

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

if ~exists("dynamic_linkwindowslib") then
    load("SCI/modules/dynamic_link/macros/windows/lib");
end

// checks that we don't have a dependency on libmmd.dll
// on other than fortran libs
allDLLs = ls("SCI/bin/*.dll");
fortran = grep(allDLLs, ["libmmd", "libifcore", "lapack", "Arpack", "arpack", "_f."]);
allDLLs(fortran) = [];

[_, output] = host(dlwWriteBatchFile("dumpbin /IMPORTS " + strcat(allDLLs, " ")));
found = grep(output, "libmmd.dll");
if found <> [] then
    names = grep(output, "Dump of");
    disp(output(gsort([found names],"g","i")))
    error("dependency found");
end

// checks that we don't have a dependency on user32.dll
allDLLs = ls("SCI/bin/*.dll");
haveDeps = grep(allDLLs, ["io", "libcrypto", "tk85", "tcl85", "sound", "scilocalization", "sciconsole", "noconsole", "newt", "nativewindow", "libjvm", "Windows", "windows", "webtools", "core", "ast", "glass", "prism_d3d"]);
allDLLs(haveDeps) = [];
[_, output] = host(dlwWriteBatchFile("dumpbin /IMPORTS " + strcat(allDLLs, " ")));
found = grep(output, "USER32.dll");
if found <> [] then
    names = grep(output, "Dump of");
    disp(output(gsort([found names],"g","i")))
    error("dependency found");
end
