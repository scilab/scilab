//Copyright INRIA

files=G_make(['/tmp/ext5f.o'],'ext5f.dll');
link(files,'ext5f');

// reading vector a in scilab internal stack
a=[1,2,3];b=[2,3,4];
c=call('ext5f',b,1,'d','out',[1,3],2,'d');
if norm(c-(a+2*b)) > %eps then pause,end
