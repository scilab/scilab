
files=['intex2f.o'];
ilib_build('gw_ex2f',['ex2f','C2F(intex2f)'],files,[]);

exec loader.sce;

// Creating the variable C 
ex2f();
if C<>[10,20,30] then pause,end


