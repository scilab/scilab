// function definition
// Copyright INRIA
deff('[x]=t5(a,b)','x=a+%i*b','n')

// define type and dimension of the input args
lt5=list();
lt5(1)=list('1','m1','n1',0);
lt5(2)=list('1','m1','n1',0);

// show the initial data
printspecs(t5,lt5)

//translate
txt=sci2for(t5,'t5',lt5);
message(['Generated Fortran code';' ';txt])
