//variable read in Scilab stack
param=1:10;
z=ex10f();
if norm(z-param) > %eps then pause,end
