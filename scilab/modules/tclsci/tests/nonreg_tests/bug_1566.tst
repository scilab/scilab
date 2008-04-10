// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre .marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1566 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1566
//
// <-- Short Description -->
//    TCL_SetVar eats imaginary part
//
//    I know, the man page for TCL_SetVar states that when used with numbers 
//    those numbers must be real. However, the following does not throw an 
//    error:
//
//    -->TCL_SetVar("A",1+%i)
//     ans  =
//
//      T
//
//    I think it should either spit an error, or fully support complex 
//    numbers.
//
//    -->TCL_GetVar("A")
//     ans  =
//
//     1.0000000000
//
//    Imaginary part disappeared! This is true for single complex numbers or 
// ...

err = execstr('TCL_SetVar(''A'',1+%i)','errcatch');
if err<> 999 then pause,end
