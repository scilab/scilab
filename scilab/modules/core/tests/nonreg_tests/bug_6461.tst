//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6461 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6461
//
// <-- Short Description -->
// Scilab External function executed by scifunction may make Scilab crash when they produce an error.
A=rand(3,3);
function s=extern1(ev)
  if %t then error(msprintf(_("%s: zsefdsdfdsddsffsd %f\n"),'fsqp',1)),end
endfunction

if execstr('[U,dim]=schur(A,extern1)','errcatch')<>10000 then pause,end

