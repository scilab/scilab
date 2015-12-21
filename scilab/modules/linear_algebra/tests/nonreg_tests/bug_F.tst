//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug F -->
//
// <-- OLD BUG -->

[q,r]=qr((1:3)');
if (r(2)<>0) | (r(3)<>0) then pause,end
