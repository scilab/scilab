// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NOT FIXED -->
//
// <-- Non-regression test for bug 5607 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5607
//
// <-- Short Description -->
// mtlb_mode did not work
  
mtlb_mode(%t);
r = [] + int32(2);
if r <> [] then pause,end