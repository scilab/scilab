//Copyright INRIA

//Passing a parameter to argument funtion of ode
files=G_make(['/tmp/ext11f.o'],'ext11f.dll');
link(files,'ext11f');

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext11f');
//param must be defined as a scilab variable upon calling ode

