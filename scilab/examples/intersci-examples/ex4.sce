//ex4 example
//1-Creating interface source file 
// from intex4.desc file by call to intersci (see Makefile)
// Making object files
//   Interface file '/tmp/ex4fi.o'
//   User's files '/tmp/ex4f.o';
files=G_make(['/tmp/ex4fi.o','/tmp/ex4f.o'],'ex4.dll');
//2-Link object files .o with addinter
//addinter(files,'intex4',intex4_funs);
exec('ex4fi.sce');
//Run Scilab functions:
sys=ssrand(2,2,3,list('co',1));
n=contrb(sys,0.01)
if n <> 1 then pause,end




