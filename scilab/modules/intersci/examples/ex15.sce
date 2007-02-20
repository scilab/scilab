//ex15 example
//1-Creating interface source file 
// Making object files
//  Interface file '/tmp/ex15fi.o';
//  User's files '/tmp/ex15c.o';
files=G_make(['/tmp/ex15fi.o','/tmp/ex15c.o';],'ex15.dll');
//2-Link object files .o with addinter
//addinter(files,'intex15',intex1_funs);
exec('ex15fi.sce');
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

