mode(-1)
// function definition
// Copyright INRIA
deff('[x]=t1(a,b,n)',[
'z=n+m+n,'
'c(1,1)=z,'
'c(2,1)=z+1,'
'c(1,2)=2,'
'c(2,2)=0,'
'if n==1 then,'
' x=a+b+a,'
'else,'
' x=a+b-a''+b,'
'end,'
'y=a(3,z+1)-x(z,5),'
'x=2*x*x*2.21,'
'sel=1:5,'
't=a*b,'
'for k=1:n,'
' z1=z*a(k+1,k)+3,'
'end,'
't(sel,5)=a(2:4,7),'
'x=[a b;-b'' a'']'],'n')

// define type and dimension of the input args
lt1=list();
lt1(1)=list('1','m','m');
lt1(2)=list('1','m','m');
lt1(3)=list('0','1','1');

// show the initial data
printspecs(t1,lt1)

//translate
txt=sci2for(t1,'t1',lt1);
message(txt)
