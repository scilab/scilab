// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
// This file is released into the public domain

mode(-1);
lines(0);

disp("cpp_find(''Scilab is a numerical computational package'',''numerical'')");
disp('position : ' + string(cpp_find('Scilab is a numerical computational package','numerical')));
disp("cpp_find(''Scilab is a numerical computational package'',''package'')");
disp('position: ' + string(cpp_find('Scilab is a numerical computational package','package')));
