// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2646 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2646
//
// <-- Short Description -->
// min() produces wrong result when a vector input contains %nan.
// The function seems to produce as result the first element that follows
// the last %nan in the vector input.

if min([ 1 2 %nan 3 4 ])      <> 1 then pause, end
if min([1 2 %nan 3 %nan 4 5]) <> 1 then pause, end
if min([%nan %nan %nan -%inf %nan]) <> -%inf then pause, end
