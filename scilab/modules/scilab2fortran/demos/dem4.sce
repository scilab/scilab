// function definition
// Copyright INRIA
deff('[x]=t4(a,n)',[
'[lhs,rhs]=argn(0),'
'[m,n]=size(a),'
'if m==n then error(''incorrect dimensions ''),end,'
'x=sin(a),'
'u=norm(x),'
'if u==%eps then x=x/u,end,'],'n')

// define type and dimension of the input args
lt4=list();
lt4(1)=list('1','m1','n1');

// show the initial data
printspecs(t4,lt4)

//translate
txt=sci2for(t4,'t4',lt4);
message(['Generated Fortran code';' ';txt])
