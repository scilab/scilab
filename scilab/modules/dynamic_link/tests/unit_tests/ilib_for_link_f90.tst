// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// Test fortran 90 dynamic link.
//
cwd = pwd()
cd TMPDIR
sourcecode=['subroutine incrdoublef90(x,y)'
    '  implicit none'
    '  double precision, intent(in) :: x'
    '  double precision, intent(out) :: y'
    '  y=x+1'
    'end subroutine incrdoublef90'];
mputl(sourcecode,'incrdoublef90.f90');
libpath=ilib_for_link('incrdoublef90','incrdoublef90.o',[],'f90','Makefile');
exec loader.sce 
n=1.
m=call("incrdoublef90",n,1,"d","out",[1,1],2,"d");
if abs(m-2.)>%eps then pause,end
chdir(cwd)


