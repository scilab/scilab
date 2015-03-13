// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4495 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4495
//
// <-- Short Description -->
// exec does not check second and third input arguments.
// =============================================================================
function mytestfunc()
    disp("mytestfunc: called ok");
endfunction
// =============================================================================
exec(mytestfunc);
// =============================================================================
mputl("a=1;b=2;",TMPDIR+"/myscript");
ierr = execstr("exec(TMPDIR+''/myscript'',''toto'',''tata'');","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
exec(TMPDIR+"/myscript");
// =============================================================================
exec(TMPDIR+"/myscript","errcatch");
exec(TMPDIR+"/myscript",-1);
exec(TMPDIR+"/myscript","errcatch",-1);
ierr = execstr("exec(TMPDIR+''/myscript'',-1,''blabla'');","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("exec(TMPDIR+''/myscript'',0,''blabla'');","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("exec(TMPDIR+''/myscript'',''blabla'',0);","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
