// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- WINDOWS ONLY -->
//
// <-- CLI SHELL MODE -->
//
info = dllinfo(SCI + "\bin\scilex.exe", "machine");
assert_checktrue(or(info(2) == ["x86","x64"]));

info = dllinfo(SCI + "\bin\api_scilab.dll", "machine");
assert_checktrue(or(info(2) == ["x86","x64"]));

info = dllinfo(SCI + "\bin\scilex.exe", "exports");
assert_checktrue(size(info) == 0);

info = dllinfo(SCI + "\bin\api_scilab.dll", "exports");
assert_checkequal(info(1), "api_scilab.dll");
assert_checktrue(size(info(2), "*") > 300);
assert_checktrue(typeof(info(2)(1)) == "string");

info = dllinfo(SCI + "\bin\scilex.exe", "imports");
assert_checktrue(size(info) > 4);

info = dllinfo(SCI + "\bin\api_scilab.dll", "imports");
assert_checktrue(size(info) > 4);
