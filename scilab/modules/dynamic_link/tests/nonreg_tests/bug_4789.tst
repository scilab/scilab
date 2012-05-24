// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4789 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4789
//
// <-- Short Description -->
// number of input arguments is not checked in ilib_xxx functions

ierr = execstr("l=ilib_for_link(''rosenf'',[''adStack.c'',''adBuffer.f'',''rosenf_b.f'']);","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("ilib_mex_build();","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("ilib_gen_Make();","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("ilib_gen_gateway();","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("ilib_compile();","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("ilib_build();","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("G_make();","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("gencompilationflags_unix();","errcatch");
if ierr <> 10000 then pause,end

