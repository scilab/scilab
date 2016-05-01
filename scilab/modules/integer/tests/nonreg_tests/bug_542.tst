//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 542 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=542
//
// <-- Short Description -->
//    Bug Report Id: 031202003109123244
//     bitwise operations among integers used to be defined until last time I needed them. How come they aren't anymore?
//    On Scilab CVS with " &, | " function
//    The Error Messages are:
//                       !--error    43 
//    not implemented in scilab....
//
//    Commands:  int8(7) & int8(8)
//    Enrico SEGRE on Linux version RH9 distribution  with   as window manager
//    Israel  
//    November 9, 2003 at 12:32:44

if execstr("int8(7) & int8(8)","errcatch")<>0 then pause,end
