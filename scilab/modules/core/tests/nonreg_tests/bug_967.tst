//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 967 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=967
//
// <-- Short Description -->
//    please read the messages posted to comp.soft-
//    sys.math.scilab with thread subject 'Nan(not-a-number), Inf
//    (infinity) question...' (1 sep 2004) and
//    'Runtime error...' (31 aug 2004)

m=[5 %inf;2 %i];

if or(m==%nan) then pause,end
