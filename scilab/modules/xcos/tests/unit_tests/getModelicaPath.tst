// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
//
// <-- Short Description -->
// Check getModelicaPath as a whitebox

// check a call without %MODELICA_USER_LIBS
clear("%MODELICA_USER_LIBS");
[modelica_path, modelica_directory] = getModelicaPath();

assert_checkequal(exists("%MODELICA_USER_LIBS"), 0);

result = modelica_path;
result = strsubst(result, '\', '/');
expectedResult = "SCI/modules/scicos_blocks/macros/" + ["Electrical","Hydraulics"];
expectedResult = strsubst(expectedResult, '\', '/');
assert_checkequal(expectedResult, result);


result = modelica_directory;
result = strsubst(result, '\', '/');
expectedResult = pathconvert(TMPDIR+"/modelica/",%T,%T);
expectedResult = strsubst(expectedResult, '\', '/');
assert_checkequal(expectedResult, result);

// check a call with %MODELICA_USER_LIBS
%MODELICA_USER_LIBS = TMPDIR;
[modelica_path, modelica_directory] = getModelicaPath();

assert_checkequal(%MODELICA_USER_LIBS, TMPDIR);

result = modelica_path;
result = strsubst(result, '\', '/');
expectedResult = ["SCI/modules/scicos_blocks/macros/" + ["Electrical","Hydraulics"]];
expectedResult = strsubst(expectedResult, '\', '/');
assert_checkequal(expectedResult, result);

result = modelica_directory;
result = strsubst(result, '\', '/');
expectedResult = pathconvert(TMPDIR+"/modelica/",%T,%T);
expectedResult = strsubst(expectedResult, '\', '/');
assert_checkequal(expectedResult, result);

// check a call with duplicate path in %MODELICA_USER_LIBS
%MODELICA_USER_LIBS = [TMPDIR, TMPDIR];
[modelica_path, modelica_directory] = getModelicaPath();

assert_checkequal(%MODELICA_USER_LIBS, [TMPDIR, TMPDIR]);

result = modelica_path;
result = strsubst(result, '\', '/');
expectedResult = ["SCI/modules/scicos_blocks/macros/" + ["Electrical","Hydraulics"]];
expectedResult = strsubst(expectedResult, '\', '/');
assert_checkequal(expectedResult, result);

result = modelica_directory;
result = strsubst(result, '\', '/');
expectedResult = pathconvert(TMPDIR+"/modelica/",%T,%T);
expectedResult = strsubst(expectedResult, '\', '/');
assert_checkequal(expectedResult, result);

