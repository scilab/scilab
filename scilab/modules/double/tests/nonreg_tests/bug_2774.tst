// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2774 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2774
//
// <-- Short Description -->
//    In Scilab 5.0 as in 4.1.2, the product 0.0 * %inf provides the incorrect value 0.0, 
//    but the product %inf * 0.0 provides the correct value %nan.

if ~isnan(0.0*%inf) then pause;end

if ~isnan(%inf*0.0) then pause;end
