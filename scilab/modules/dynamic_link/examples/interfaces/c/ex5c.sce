
files=['intex5c.o','crestrc.o'];
ilib_build('gw_ex5c',['ex5c','intex5c'],files,[]);

exec loader.sce;

//Character string created by C function
x3=ex5c();
if x3<>"Scilab is ..." then pause,end