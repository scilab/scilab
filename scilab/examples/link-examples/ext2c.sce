//Copyright INRIA

//Simple example #2

files=G_make(['/tmp/ext2c.o'],'ext2c.dll');
link(files,'ext2c','C');

a=[1,2,3];b=[4,5,6];n=3;
c=call('ext2c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(sin(a)+cos(b))) > %eps then pause,end
