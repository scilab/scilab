//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA -Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2131 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2131
//
// <-- Short Description -->
//Inconsistent results from functions median() with given argument of [].

if ~isnan(median([])) then pause,end
if ~isempty(median([],1)) then pause,end
if ~isempty(median([],2)) then pause,end
