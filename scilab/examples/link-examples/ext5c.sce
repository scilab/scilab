//Copyright INRIA

files=G_make(['/tmp/ext5c.o'],'ext5c.dll');
link(files,'ext5c','C');

// reading vector a in scilab internal stack
Amatrix=[1,2,3];b=[2,3,4];
c=call('ext5c',b,1,'d','out',[1,3],2,'d');
if norm(c-(Amatrix+2*b)) > %eps then pause,end

