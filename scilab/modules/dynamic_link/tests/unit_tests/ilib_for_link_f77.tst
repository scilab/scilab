// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
// <-- CLI SHELL MODE -->
//
// Test fortran 77 dynamic link.
//

ilib_verbose(0);

cwd = pwd();
cd TMPDIR;
sourcecode=['      subroutine incrdoublef77(x,y)'
    '      implicit none'
    '      double precision x'
    '      double precision y'
    '      y=x+1'
    '      end'];
mputl(sourcecode,'incrdoublef77.f');
libpath=ilib_for_link('incrdoublef77','incrdoublef77.f',[],'f',[]);
exec loader.sce;
n = 1;
m = call("incrdoublef77",n,1,"d","out",[1,1],2,"d");
if abs(m-2.)>%eps then pause,end
chdir(cwd);


