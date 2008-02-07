
files=['intex13c.o'];
ilib_build('gw_ex13c',['ex13c','intex13c'],files,[]);

exec loader.sce;

a=ex13c([1,2,3]+%i*[4,5,5]);
if norm(a-2*[1,2,3]-%i*[4,5,5]*3) > %eps then pause,end

a=ex13c([1,2,3]);
if norm(a-2*[1,2,3]) > %eps then pause,end





