
files=['intex15c.o'];
ilib_build('gw_ex15c',['ex15c','intex15c'],files,[]);

exec loader.sce;

// Accessing a Scilab string inside an interface 
Mystr='My string';
ex15c()

