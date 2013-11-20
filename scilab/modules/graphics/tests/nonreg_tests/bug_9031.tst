// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 9031 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9031
//
// <-- Short Description -->
// Misalignment when using xstring with a matrix

clf();

// Observe that all the string are aligned on the same baseline
xstring(0.2, 0.5,["$\overbrace{Scilab}$" "n''est ";"pas" "$\underbrace{Matlab}$"])
xstring(0.2, 0.4, ["|_" , "_"]);
xstring(0.2, 0.3, ["|-" , "-"]);
xstring(0.2, 0.2, ["hp" , "a"]);
xstring(0.2, 0.1,["Scilab" "n''est ";"pas" "Octave"]);
