//ex2 example
//1-Creating interface source file 
// from intex2.desc file by call to intersci (see Makefile)
//  Making object files
//  Interface file '/tmp/ex2fi.o'
//  User's files ['/tmp/ex2f.o']
files=G_make(['/tmp/ex2fi.o','/tmp/ex2f.o'],'ex2.dll');
//2-Link object files .o with addinter
//addinter(files,'intex2',intex2_funs);
exec('ex2fi.sce');
//Run Scilab functions:
a=1:20;b=1:10;
d=som(a=1:20,b=1:10);
d1=ones(1,10) + a(1:10)+b(1:10);
if norm(d-d1') > %eps then pause,end;



