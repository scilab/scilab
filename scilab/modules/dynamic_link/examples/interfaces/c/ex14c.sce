
files=['intex14c.o'];
ilib_build('gw_ex14c',['ex14c','intex14c'],files,[]);

exec loader.sce;

// Creating the variable C 
ex14c();
if C<>[10,20,30] then pause,end


