//Copyright INRIA

//creating vector c in scilab internal stack

files=G_make(['/tmp/ext7f.o'],'ext7f.dll');
link(files,'ext7f');

a=[1,2,3]; b=[2,3,4];
//c does not exist (c made by ext7f)
c1=call('ext7f',a,1,'d',b,2,'d','out',2);
if norm(c1-b) > %eps then pause,end
//c now exists
if norm(c-(a+2*b)) > %eps then pause,end
//d exists 
if d<>"test" then pause,end

