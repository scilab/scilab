// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2430 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2430
//
// <-- Short Description -->
//    Wrong results when solving linear systems when the matrix
//    is sparse and 1x1, and rhs is compatible and sparse. 
//    In the example below, M\b and full(M)\b give a wrong result,
//    whereas M\full(b) and full(M)\full(b) are ok. 
//
//    Strangely, when the size of b is less than 10, then all
//    cases give the correct answer !

b = sparse(1:16);
if or(abs(full(2\b)-2\full(b)) >= 100*%eps) then pause,end
