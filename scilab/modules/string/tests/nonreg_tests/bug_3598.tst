// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3598 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3598
//
// Short description:
// stripblanks(1) returns "stripblank : Pas assez de mémoire."

// <-- CLI SHELL MODE -->

//==============================================================
ierr = execstr("r = stripblanks(1);","errcatch");
if ierr <> 999 then pause,end
//==============================================================
ierr = execstr("r = cd(1);","errcatch");
if ierr <> 999 then pause,end
//==============================================================
ierr = execstr("r = ls(1);","errcatch");
if ierr <> 999 then pause,end
//==============================================================
ierr = execstr("r = dir(1);","errcatch");
if ierr <> 999 then pause,end
//==============================================================
ierr = execstr("r = help(1);","errcatch");
if ierr <> 999 then pause,end
//==============================================================

