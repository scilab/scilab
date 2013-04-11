// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3115 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3115
//
// <-- Short Description -->
// In log mode the ticks locations properties are wrong in scilab5:

clf()
plot([0.01,100],[1,10]);a=gca();a.log_flags='lln';

// check that ticks locations are right
if ( a.x_ticks.locations <> [0.01;0.1;1;10;100] ) then pause; end
if ( a.y_ticks.locations <> [1;2;4;6;8;10] ) then pause; end
