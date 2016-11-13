// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4574 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4574
//
// <-- Short Description -->
// ilib_build does not check if library is already loaded in scilab.

ilib_verbose(0);
chdir(TMPDIR);

i=['#include ""api_scilab.h""'
   'int intfun1(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)'
   '{'
   '  return 0;'
   '}'];
mputl(i,'intfun1.c');

ilib_build('foo',['scifun1','intfun1'],'intfun1.c',[]);
exec loader.sce;

i=['#include ""api_scilab.h""'
   'int intfun1(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)'
   '{'
   '  return 1;'
   '}'];
mputl(i,'intfun2.c');

cmdstr = "ilib_build(''foo'',[''scifun1'',''intfun2''],''intfun2.c'',[]);"
ierr = execstr(cmdstr, 'errcatch');
if ierr <> 999 then pause,end

