//ex12 example
//1-Creating interface source file 
// Making object files
// Interface file '/tmp/ex12fi.o'
// User's files '/tmp/ex12c.o';
files=G_make(['/tmp/ex12fi.o','/tmp/ex12c.o'],'ex12.dll');
//2-Link object files .o with addinter
//addinter(files,'intex12',intex1_funs);
exec('ex12fi.sce');
//Run Scilab functions:
b=ccalc12();

if norm(b-(0:9)) > %eps then pause,end




