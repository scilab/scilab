// Making object files
// Interface file
ifile='zallfi.o';
ufiles=['/tmp/ex1c.o','/tmp/ex2f.o','/tmp/ex4f.o','/tmp/ex5f.o','/tmp/ex6c.o','/tmp/ex7f.o','/tmp/ex8c.o','/tmp/ex9f.o','/tmp/ex10f.o','/tmp/ex11c.o','/tmp/ex12c.o','/tmp/ex13c.o','/tmp/ex14c.o','/tmp/ex15c.o','/tmp/ex16c.o'];
files=G_make([ifile,ufiles],'zallfi.dll');
//2-Link object files .o with addinter
//addinter(files,'intex1',intex1_funs);
exec('zallfi.sce');
//Run Scilab functions:
a=[1,2,3];b=[4,5,6];
c=ext1c(a,b);
if norm(c-(a+b)) > %eps then pause,end


//Run Scilab functions:
a=1:20;b=1:10;
d=som(a=1:20,b=1:10);
d1=ones(1,10) + a(1:10)+b(1:10);
if norm(d-d1') > %eps then pause,end;



//Run Scilab functions:
n=3;a=13;incx=2;incy=3;x=1:10;y=2*x;
ynew=scilabdaxpy(n,a,x,incx,y,incy);
y(1:incy:n*incy)=y(1:incy:n*incy)+a*x(1:incx:n*incx);
if norm(ynew-y) > %eps then pause,end

//Run Scilab functions:
sys=ssrand(2,2,3,list('co',1));
n=contrb(sys,0.01)
if n <> 1 then pause,end

//Run Scilab functions:
a=1:5;b=-a;c=ones(3,3);
[a,b,c,d]=foobar('mul',a,b,c)
[a,b,c,d]=foobar('add',a,b,c)


w=['this' 'is'; 'naw' 'gaad']
rep=calc6a(w);
if rep<>['this' 'is'; 'now' 'good'] then pause,end;

//calc5: creation of a character string matrix.
a=calc6b();
a1='char '; a1=matrix(a1(ones(1,6))+string(0:5),3,2);
if a<>a1 then pause,end

//Run Scilab functions:
if calc('one')<>1 then pause,end
if calc('two')<>2 then pause,end
if calc('other')<>-1 then pause,end


//calc8: matrix of integer type created by C function (malloc and free).
a=calc8();
if norm(a - matrix(0:14,3,5)) > %eps then pause,end

//calc7: function with two optional values, a string and a scilab variable.
bb=33;  //scilab optional variable
[a,b,c,d]=calc9(10);
if c<>'toto' then pause,end
if norm(d-[2,4,6,8])> %eps then pause,end
if b<>bb then pause,end

//Test Scilab functions:
// calc1: 3 matrix input variables and at most 3 output variables of
// types double, real, int
l=calc10(list(1,[2,3],[1,2;3,4]));
if norm(l(1)-2) > %eps then pause,end
if norm(l(2)-([2,3]+[1,2])) > %eps then pause,end
if norm(l(3)-([1,2;3,4]+[1,3;2,4])) > %eps then pause,end

//Run Scilab functions:
a=[1,2,3]+%i*[4,5,6];
b=ext11c(a);
if norm(b-([4,5,6]+%i*[1,2,3])) > %eps then pause,end


//Run Scilab functions:
b=ccalc12();
if norm(b-(0:9)) > %eps then pause,end

//Run Scilab functions:
a=rand(2,3);
b=ext13c(a);
if norm(b- matrix(2*a,3,2)) > %eps then pause,end

//Run Scilab functions:
a=[0,0,1.23;0,2.32,0;3.45,0,0];
ai=[0,0,9;0,6,0;7,0,0];
spa=sparse(a);
spai=sparse(a+%i*ai);
// simple sparse argument 

b=spt1(spa);
if norm(full(b- spa)) > %eps then pause,end

b=spt1(spai);
if norm(full(b- spai)) > %eps then pause,end

// sparse argument + conversion to int 

b=spt2(spa);
if norm(full(b- int(spa))) > %eps then pause,end

b=spt2(spai);
if norm(full(b- int(spai))) > %eps then pause,end

// sparse and return a sparse in a list 

b=spt3(spa);
if norm(full(b(1)- spa)) > %eps then pause,end

b=spt3(spai);
if norm(full(b(1)- spai)) > %eps then pause,end

// new sparse in intersci 

b=spt4(spa);
if norm(full(b- 2*spa)) > %eps then pause,end

b=spt4(spai);
if norm(full(b- 2*spai)) > %eps then pause,end

// new sparse + conversion to int

b=spt5(spa);
if norm(full(b- int(2*spa))) > %eps then pause,end

b=spt5(spai);
if norm(full(b- int(2*spai))) > %eps then pause,end

// new sparse returned in a list 

b=spt6(spa);
if norm(full(b(1)- 2*spa)) > %eps then pause,end

b=spt6(spai);
if norm(full(b(1)- 2*spai)) > %eps then pause,end

// list argument with a sparse 

b=spt7(list(spa));
if norm(full(b- spa)) > %eps then pause,end

b=spt7(list(spai));
if norm(full(b- spai)) > %eps then pause,end

// list argument + conversion 

b=spt8(list(spa));
if norm(full(b- int(spa))) > %eps then pause,end

b=spt8(list(spai));
if norm(full(b- int(spai))) > %eps then pause,end

// list argument + list output 

b=spt9(list(spa));
if norm(full(b(1)- spa)) > %eps then pause,end

b=spt9(list(spai));
if norm(full(b(1)- spai)) > %eps then pause,end

b=spt10(spa);
if norm(full(b- 2*spa)) > %eps then pause,end


//Run Scilab functions:
a=[0,0,1.23;0,2.32,0;3.45,0,0];

// simple matrix argument 

b=mat1(a);
if norm(b- a) > %eps then pause,end

// matrix argument + conversion to int 

b=mat2(a);
if norm(b- int(a)) > %eps then pause,end

// matrix and return a matrix in a list 

b=mat3(a);
if norm(b(1)- a) > %eps then pause,end

// new matrix in intersci 

b=mat4(a);
if norm(b- 2*a) > %eps then pause,end

// new matrix + conversion to int

b=mat5(a);
if norm(b- int(2*a)) > %eps then pause,end

// new matrix returned in a list 

b=mat6(a);
if norm(b(1)- 2*a) > %eps then pause,end

// list argument with a matrix  

b=mat7(list(a));
if norm(b- a) > %eps then pause,end

// list argument + conversion 

b=mat8(list(a));
if norm(b- int(a)) > %eps then pause,end

// list argument + list output 

b=mat9(list(a));
if norm(b(1)- a) > %eps then pause,end

// cintf 

b=mat10();
if norm(b'- (0:9)) > %eps then pause,end

// cintf + list 

b=mat11();
if norm(b(1)'- (0:9)) > %eps then pause,end

