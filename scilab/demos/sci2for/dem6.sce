// function definition
// Copyright INRIA
deff('[x]=t(a,b)',[
'c=b*b,'
'x=(a/b)/2+2/c,'],'n')

// define type and dimension of the input args
lt=list();
lt(1)=list('1','na','ma');
lt(2)=list('1','ma','ma');

// show the initial data
printspecs(t,lt)

//translate
txt=sci2for(t,'t',lt);
message(['Generated Fortran code';' ';txt])
