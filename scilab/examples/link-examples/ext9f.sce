//Copyright INRIA

//argument function for ode
//call ext9f argument function with dynamic link

files=G_make(['/tmp/ext9f.o'],'ext9f.dll');
link(files,'ext9f');

ode([1;0;0],0,[0.4,4],'ext9f')
