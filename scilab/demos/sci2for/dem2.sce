// function definition
// Copyright INRIA
deff('[x]=t2(a,b,c,d)',[
'z=[a b b],'
'y=[a;c;c],'
'x=[a b;c d]*3.5'],'n')

// define type and dimension of the input args
lt2=list();
lt2(1)=list('1','m1','n1');
lt2(2)=list('1','m1','n2');
lt2(3)=list('1','m2','n1');
lt2(4)=list('1','m2','n2');

// show the initial data
printspecs(t2,lt2)

//translate
txt=sci2for(t2,'t2',lt2);
message(txt)
