// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 55 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=55
//
// <-- Short Description -->
//    The sci2exp function gives wrong result with the -%inf 
//    value.
//    sci2exp(-%inf) = -Inf instead of the right result = -%inf

if sci2exp(%inf) <> "%inf"  then pause,end
if sci2exp(-%inf)<> "-%inf" then pause,end
if sci2exp(%nan) <> "%nan"  then pause,end
if sci2exp(-%nan)<> "%nan"  then pause,end
