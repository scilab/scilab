//Copyright INRIA

//(very) simple example 1

files=G_make(['/tmp/ext1f.o'],'ext1f.dll');
link(files,'ext1f');

a=[1,2,3];b=[4,5,6];n=3;
c=call('ext1f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end

