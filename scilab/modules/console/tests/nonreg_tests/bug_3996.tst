// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3996 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3996
//
// <-- Short Description -->
// tabulation included in a paste string may make scilab crash in -nw or -nwni mode

// <-- INTERACTIVE TEST -->

deff('foo','a=aaaa')
try
  foo();
  l=0	;//here there is a tabulation between 0 and ;
end
