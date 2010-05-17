// ====================================================================
// Copyright DIGITEO 2008
// Allan CORNET
// This file is released into the public domain
// ====================================================================
mode(-1);
lines(0);

disp("cpp_find(''Scilab is a numerical computational package'',''numerical'')");
disp('position : ' + string(cpp_find('Scilab is a numerical computational package','numerical')));
disp("cpp_find(''Scilab is a numerical computational package'',''package'')");
disp('position: ' + string(cpp_find('Scilab is a numerical computational package','package')));

// ====================================================================
