// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 3940 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3940
//
// <-- Short Description -->
// uigetdir returns a invalid value if you do "cancel"


%newDir = uigetdir(pwd(), gettext("Select a directory"))
// click on cancel

if type(%newDir)<> 10 then pause,end

if isempty(%newDir)<> %t then pause,end
if length(%newDir)<> 0 then pause,end
