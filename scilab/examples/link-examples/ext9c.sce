//Copyright INRIA

//argument function for ode
//call ext9c argument function with dynamic link

files=G_make(['/tmp/ext9c.o'],'ext9c.dll');
link(files,'ext9c','C');

ode([1;0;0],0,[0.4,4],'ext9c')
