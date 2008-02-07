
files=['intex4c.o'];
ilib_build('gw_ex4c',['ex4c','intex4c'],files,[]);

exec loader.sce;

//matrix (double) created by C function
x1=ex4c();
if norm(x1-matrix((1:5*3),3,5)) > %eps then pause,end
