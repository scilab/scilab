
//We build a function ex17c with 3 input parameters X,Y,f and
//1 output parameter Z: Z=ex17c(X,Y,f).
//The third input parameter of ex17c (f) is a function.
//This function, which is passed to ex17c as an argument,
//is called a "argument function".
//This function can be a Scilab function or a C function.
//In both cases, this function must have a specific calling 
//sequence which is imposed by ex17c. 
// (A typical example is the case of an ode solver:
//  if we want to solve an ode xdot=f(x), we want to build a function
//  ode with a syntax like y=ode(x0,t,f) where x0 is a real vector, 
//  t a real number and f the "argument function" f: x->f(x). )

//There are two ways of calling ex17c:
//Z=ex17c(X,Y,f) for computing Z from X and Y using the Scilab function f

//Z=ex17c(X,Y,"f") for computing Z from X and Y using the C function "f"
//The C function f can be dynamically linked to Scilab, using the
//link command (e.g. if f=f3c, link("f3c.o","f3c","C") ) or it can be put in
//a table of functions (see ex17c.c).

//In the example proposed here, we want to build a function ex17c with
//syntax Z=ex17c(X,Y,f) which returns a real matrix Z obtained from 
//the two real input vectors X and Y by a calculation performed by
//f. To be specific, we want Z(i,j)=f(X(i),Y(j)). In this example,
//the argument function f is a function from R x R to R.
//The Scilab built-in function function feval does this: ex17c emulates
//feval.

//If f is a Scilab function, its syntax is z=f(x,y), defined for instance 
//by:
//deff('z=f(x,y)','z=cos(x+y)');
//
//If f is a C function, its definition will be:
//int f(x,y,z) 
//     double x,y,*z;
//{
//  *z= cos(x+y);
//}
//Note that the argument function f must be defined as above, defining f as e.g.
//int f(z,x,y)
//is not allowed.

//If X is a vector with m components and Y is a vector with n components,
//the resulting matrix Z=ext17(X,Y) is m x n.

//The interface function is intex17c.

//Test inputs for ex17c:
X=1:3;Y=4:6;
//...and 3 examples of Scilab argument functions:
deff('z=f1(x,y)','z=x+y');
deff('z=f2(x,y)','z=x*x+y*y');
deff('z=f3(x,y)','z=cos(x+y)');

//reference values for Z calculated by feval
Zref1=feval(X,Y,f1);
Zref2=feval(X,Y,f2);
Zref3=feval(X,Y,f3);

// Calling ex17c with a argument function which is a Scilab function:
Z1=ex17c(X,Y,f1);
if norm(Z1-Zref1) > %eps then pause,end

Z2=ex17c(X,Y,f2);
if norm(Z2-Zref2) > %eps then pause,end

Z3=ex17c(X,Y,f3);
if norm(Z3-Zref3) > %eps then pause,end

// Calling ex17c with a argument function which is a C function:
Z1=ex17c(X,Y,'f1c');     // f1c defined in ex17c.c
if norm(Z1 - Zref1) > %eps then pause,end

Z2=ex17c(X,Y,'f2c');   // f2c defined in ex17c.c
if norm(Z2 - Zref2) > %eps then pause,end

// making f3c.o and linking f3c dynamically with Scilab
if ~c_link('f3c') then 
  files=G_make(['f3c.o'],'f3c.dll');
  link(files,'f3c','C') ;
end

// Calling ex17c with the C argument function fp3
Z3=ex17c(X,Y,'f3c');
if norm(Z3 - Zref3) > %eps then pause,end





