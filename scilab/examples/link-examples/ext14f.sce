//Copyright INRIA

//(very) simple example 1

files=G_make(['/tmp/ext14f.o'],'ext14f.dll');
link(files,'ext14fi');

a=[1,2,3];b=[4,5,6];n=3;
c=fort('ext14fi',a,b);
if norm(c-(a+b)) > %eps then pause,end


