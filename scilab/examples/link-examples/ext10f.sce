//Copyright INRIA

files=G_make(['/tmp/ext10f.o'],'ext10f.dll');
link(files,'ext10f');

//passing a parameter to ext10f routine by a list:
param=[0.04,10000,3d+7];    
y=ode([1;0;0],0,[0.4,4],list('ext10f',param))
