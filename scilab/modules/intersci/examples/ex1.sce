//ex1 example
//1-Creating interface source file 
//  from ex1fi.desc file by call to intersci (see Makefile)
//  Making object files 
files=G_make(['/tmp/ex1fi.o','/tmp/ex1c.o'],'ex1.dll');
//2-Link object files .o with addinter
//addinter(files,'intex1',intex1_funs);
exec('ex1fi.sce');
//Run Scilab functions:
a=[1,2,3];b=[4,5,6];
c=ext1c(a,b);
if norm(c-(a+b)) > %eps then pause,end


