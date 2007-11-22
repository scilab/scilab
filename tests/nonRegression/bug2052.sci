// <-- Non-regression test for bug 2052 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2052
//
// <-- Short Description -->
//    Bug dans la primitive "isequal" (au moins) avec des type de données 
//    utilisateur


// Non-regression test file for bug 2052
// Serge Steer - Scilab Project
// Copyright INRIA
// 19/02/2007

mode(-1);
clear;
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
r=isequal(a,a)&~isequal(a,A)&~isequal(A,a)&isequal(A,A)&isequal(l,l)&~isequal(l,A);

affich_result(r,2052);
clear
