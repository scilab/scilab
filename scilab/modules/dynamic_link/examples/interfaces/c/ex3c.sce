
files=['intex3c.o'];
ilib_build('gw_ex3c',['ex3c','intex3c'],files,[]);

exec loader.sce;


A = rand(10,10,'n')>=0 ;

B=ex3c(A);

