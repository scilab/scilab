//Copyright INRIA

//same example as # 10 with call to matptr
//param must be defined as a scilab variable

files=G_make(['/tmp/ext12f.o'],'ext12f.dll');
link(files,'ext12f');

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext12f');


