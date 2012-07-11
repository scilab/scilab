// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4548 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4548
//
// <-- Short Description -->
// 

ilib_verbose(0);
chdir(TMPDIR);
src = ['      subroutine intsifdec()'
'      include ''stack.h'''
'      logical getrhsvar,createvar'
'      logical checklhs,checkrhs'
'      external getrhsvar,createvar, checklhs,checkrhs'
'      character fname*6'
'      integer lunit,mode(2)'
'      character tmp*128'
'c'
'      fname=''sifdec'''
'c'
'      if(.not.checkrhs(fname,1,1)) return'
'      if(.not.checklhs(fname,1,1)) return'
'c'
'      if(.not.getrhsvar(1,''c'', M, N, lPi)) return'
'      ni=m*n'
''
'c'
'c     input file'
'      mode(1)=1'
'      mode(2)=0'
'      lunit=0'
'      call clunit(lunit,cstk(lPi:lPi+ni-1),mode)'
'      if(err.gt.0) goto  99'
'      print *,lunit,cstk(lPi:lPi+ni-1)'
'      read(lunit,''(A64)'',err=100) tmp'
'      print *,tmp'
''
''
'      lhsvar(1)=0'
'      return'
'c'
' 99   continue'
'      call error(err)'
'      return'
' 100  buf=''Erreur de lecture'''
'      call error(999)'
'      return'
'      end'
''];


mputl(src,'intsifdec.f');
files=['intsifdec.f'];
if getos() == 'Windows' then
  [a,b]=getversion();
  if grep(b,'debug')<> [] then
    setenv('DEBUG_SCILAB_DYNAMIC_LINK','YES');
  end
end
ilib_build('gwsifdec',['sifdec','intsifdec_'],files,[]);
exec loader.sce;
sifdec('SCI/modules/dynamic_link/tests/nonreg_tests/bug_4548');
