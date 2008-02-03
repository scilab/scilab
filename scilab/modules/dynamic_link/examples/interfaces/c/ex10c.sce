files=['intex10c.o'];
ilib_build('gw_ex10c',['ex10c','intex10c'],files,[]);

exec loader.sce;


//variable read in Scilab stack
param=1:10;
z=ex10c();
if norm(z-param) > %eps then pause,end
