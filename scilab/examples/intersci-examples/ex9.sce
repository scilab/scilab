//1-Creating interface source file (matusr.f)
// from ex9fi.desc file by call to intersci
// Making object files
//   Interface file '/tmp/ex9fi.o'
//   User's files '/tmp/ex9f.o';
files=G_make(['/tmp/ex9fi.o','/tmp/ex9f.o'],'ex9.dll');
//2-Link object files .o with addinter
//addinter(files,'ex9fi',matusr_funs);
exec('ex9fi.sce');
//Test Scilab functions:
//calc7: function with two optional values, a string and a scilab variable.
bb=33;  //scilab optional variable
[a,b,c,d]=calc9(10);
if c<>'toto' then pause,end
if norm(d-[2,4,6,8])> %eps then pause,end
if b<>bb then pause,end










