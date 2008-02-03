
files=['intex3f.o'];
ilib_build('gw_ex3f',['ex3f','C2F(intex3f)'],files,[]);

exec loader.sce;

// Accessing a Scilab string inside an interface 
Mystr='My string';
ex3f()

