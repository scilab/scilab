//Copyright INRIA

//Simple example #2

files=G_make(['/tmp/ext2f.o'],'ext2f.dll');
link(files,'ext2f');

a=[1,2,3];b=[4,5,6];n=3;
c=call('ext2f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(sin(a)+cos(b))) > %eps then pause,end
