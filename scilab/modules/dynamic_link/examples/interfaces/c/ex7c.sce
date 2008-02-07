
files=['intex7c.o'];
ilib_build('gw_ex7c',['ex7c','intex7c'],files,[]);

exec loader.sce;

A=[1,2,3,4];
B=[10,20,30;40,50,60];
//Just display A and B
ex7c(A,B)

