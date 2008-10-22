// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2052 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2052
//
// <-- Short Description -->
//    Bug dans la primitive "isequal" (au moins) avec des type de donn�es 
//    utilisateur

// Serge Steer - Scilab Project
// Copyright INRIA
// 19/02/2007

cur=pwd();
cd(TMPDIR);
//first create a function changetype 
code=['#include ""stack-c.h""'
'int intchangetype(fname)'
'   char *fname;'
'{'
' int m1,n1,l1;'
' CheckRhs(2,2);'
' CheckLhs(1,1);'
' GetRhsVar(2,""i"",&m1,&n1,&l1)'
' /* change the type */'
' *istk(iadr(*Lstk(Top-1)))=*istk(l1);'
' C2F(intersci).ntypes[0] = ''$'';'
' LhsVar(1)=1;'
' return 0;'
'}'];
mputl(code,'intchangetype.c');
ilib_build('libtest',['changetype','intchangetype'],'intchangetype',[]);
exec('loader.sce')
cd(cur);


//create variables of type 222
A=rand(3,2);
a=changetype(A,222);
//associate a typename to 222
typename("bug2052", 222)
//define overloading function for isequal
function r=%bug2052_isequal(x,y)
  r=type(x)==222&type(y)==222
  if r then
    x=changetype(x,1)
    y=changetype(y,1)
    r=r&and(x==y)
  end
endfunction
//Check if everything is ok
l=list(1,2,3);

if ~isequal(a,a) then pause,end
if  isequal(a,A) then pause,end
if  isequal(A,a) then pause,end
if ~isequal(A,A) then pause,end
if ~isequal(l,l) then pause,end
if  isequal(l,A) then pause,end
