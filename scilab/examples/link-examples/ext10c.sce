//Copyright INRIA

files=G_make(['/tmp/ext10c.o'],'ext10c.dll');
link(files,'ext10c','C');

//passing a parameter to ext10c routine by a list:
param=[0.04,10000,3d+7];    
y=ode([1;0;0],0,[0.4,4],list('ext10c',param))
