// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug I -->
//
// <-- OLD BUG -->
//

result = execstr("comp(beurk)","errcatch","n");
if result <> 4 then pause,end
