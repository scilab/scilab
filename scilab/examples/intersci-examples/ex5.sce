//foobar example
//1-Creating interface source file 
// from ex5fi.desc file by call to intersci (see Makefile)
//2-Making object files
// Interface file '/tmp/ex5fi.o'
// User's files '/tmp/ex5f.o';
files=G_make(['/tmp/ex5fi.o','/tmp/ex5f.o'],'ex5.dll');
//2-Link object files .o with addinter
//addinter(files,'foobar',ex5fi_funs);
exec('ex5fi.sce');
//Run Scilab functions:
a=1:5;b=-a;c=ones(3,3);
[a,b,c,d]=foobar('mul',a,b,c)
[a,b,c,d]=foobar('add',a,b,c)


