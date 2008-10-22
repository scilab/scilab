// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2006 - INRIA - Farid Belahcene
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1725 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1725
//
// <-- Short Description -->
//    the function mtlb_axis returns an index error when called with an 
//    argument of [ 1 500 -22.3 1.4] or any other argument.
//
//    This was traced to varargout(1) not being set to a.data_bounds within 
//    the code segment
//    if rhs>0 then
//
//        statements
//
//    else// v = axis
//    else
//      varargout(1)=a.data_bounds;
//    end
//    endfunction
//
//    Changing this to 
//    if rhs>0 then
//
//        statements
//      varargout(1)=a.data_bounds;
// ...

a = mtlb_axis([ 1 500 -22.3 1.4]);
if or(a <> [1 500 -22.3 1.4]) then pause,end
