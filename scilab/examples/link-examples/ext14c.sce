//Copyright INRIA

//(very) simple example 1

files=G_make(['/tmp/ext14c.o'],'ext14c.dll');
link(files,'ext14cI','C');

a=[1,2,3];b=[4,5,6];n=3;
c=fort('ext14cI',a,b);
if norm(c-(a+b)) > %eps then pause,end


