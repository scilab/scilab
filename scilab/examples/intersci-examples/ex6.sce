//1-Creating interface source file
// from ex6fi.desc file by call to intersci (see Makefile)
//2-Making object files
// Interface file '/tmp/ex6fi.o'
// User's files '/tmp/ex6c.o';
files=G_make(['/tmp/ex6fi.o','/tmp/ex6c.o'],'ex6.dll');
//2-Link object files .o with addinter
//addinter(files,'ex6fi',matusr_funs);
exec('ex6fi.sce');
//Test Scilab functions:

//calc4: input: matrix of character string 
//       output: matrix of character string (C function ccalc4)
w=['this' 'is'; 'naw' 'gaad']
rep=calc6a(w);
if rep<>['this' 'is'; 'now' 'good'] then pause,end;

//calc5: creation of a character string matrix.
a=calc6b();
a1='char '; a1=matrix(a1(ones(1,6))+string(0:5),3,2);
if a<>a1 then pause,end






