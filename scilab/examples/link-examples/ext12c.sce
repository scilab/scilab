//Copyright INRIA

//same example as # 10 with call to matptr
//param must be defined as a scilab variable

files=G_make(['/tmp/ext12c.o'],'ext12c.dll');
link(files,'ext12c','C');

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext12c');


