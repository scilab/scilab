//Copyright INRIA

//Passing a parameter to argument funtion of ode
files=G_make(['/tmp/ext11c.o'],'ext11c.dll');
link(files,'ext11c','C');

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext11c')
//param must be defined as a scilab variable upon calling ode

