// function definition
// Copyright INRIA
deff('[x]=t3(a,b,c,d)','x=[a;c]/[a b;c d]*3.5','n')

// define type and dimension of the input args
lt3=list();
lt3(1)=list('1','m1','n1');
lt3(2)=list('1','m1','n2');
lt3(3)=list('1','m2','n1');
lt3(4)=list('1','m2','n2');

// show the initial data
printspecs(t3,lt3)

//translate
txt=sci2for(t3,'t3',lt3);
message(txt)
