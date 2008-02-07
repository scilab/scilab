
files=['intex1f.o'];
ilib_build('gw_ex1f',['ex1f','C2F(intex1f)'],files,[]);

exec loader.sce;

A=[1,2,3,4];
B=[10,20,30;40,50,60];
//Just display A and B
ex1f(A,B)


