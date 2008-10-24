// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2429 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2429
//
// <-- Short Description -->
//    Implicit vector in for loop are allocated

S=stacksize();

function test(),for k=1:3d5,end,endfunction

stacksize(2d5)
if execstr('test()','errcatch') <> 0  then pause,end
stacksize(S(1))
