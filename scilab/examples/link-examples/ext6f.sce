//Copyright INRIA

//reading  vector with name='a' in scilab internal stack

files=G_make(['/tmp/ext6f.o'],'ext6f.dll');
link(files,'ext6f');

a=[1,2,3];b=[2,3,4];
c=call('ext6f','a',1,'c',b,2,'d','out',[1,3],3,'d');
if norm(c-(a+2*b)) > %eps then pause,end

