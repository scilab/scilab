// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9141 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9141
//
// <-- Short Description -->
// An extra empty line was present in the console

//Just type the following line
for i=1:10
a=1;
end
// you should see something like
//
//-->for i=1:10
//-->a=1
//-->end
//
//no empty lines between for..., a..., and end.