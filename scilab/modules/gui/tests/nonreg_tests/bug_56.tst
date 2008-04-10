// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 56 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=56
//
// <-- Short Description -->
//    findobj does not seem to work, if the
// value contains upper-case letters: e.g. the following does 
// not give the result 1 as desired


f = figure(1);
h = uicontrol("tag","F");
if isempty(findobj("tag","F")) then pause; end
if findobj("tag","F")<>h then pause; end
