// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 7691 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7691
//
// <-- Short Description -->
// Giving complex arguments to inverse degree trigonometric functions yields inconsistent results.

funmat = [
  "acosd"
  "acotd"
  "asind"
  "atand"
  "cosd"
  "cotd"
  "cscd"
  "secd"
  "sind"
  "tand"
];
for fname = funmat'
  instr = fname + "(%i)";
  execstr(instr,"errcatch");
  errmsg = lasterror();
  expected = fname + ": Wrong type for input argument #1: Real matrix expected.";
  if ( errmsg <> expected) then pause, end
end
