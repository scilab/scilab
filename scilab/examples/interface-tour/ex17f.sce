//Test inputs for ex17f:
X=1:3;Y=4:6;
//...and 3 examples of Scilab argument functions:
deff('z=f1(x,y)','z=x+y');
deff('z=f2(x,y)','z=x*x+y*y');
deff('z=f3(x,y)','z=cos(x+y)');

//reference values for Z calculated by feval
Zref1=feval(X,Y,f1);
Zref2=feval(X,Y,f2);
Zref3=feval(X,Y,f3);

// Calling ex17f with a argument function which is a Scilab function:
Z1=ex17f(X,Y,f1);
if norm(Z1-Zref1) > %eps then pause,end

Z2=ex17f(X,Y,f2);
if norm(Z2-Zref2) > %eps then pause,end

Z3=ex17f(X,Y,f3);
if norm(Z3-Zref3) > %eps then pause,end

// Calling ex17f with a argument function which is a Fortran function:
Z1=ex17f(X,Y,'f1f');     // f1f defined in ex17f.f
if norm(Z1 - Zref1) > %eps then pause,end

Z2=ex17f(X,Y,'f2f');   // f2f defined in ex17f.f
if norm(Z2 - Zref2) > %eps then pause,end

// making f3f.o and linking f3f dynamically with Scilab
if ~c_link('f3f') then 
  files=G_make(['f3f.o'],'f3f.dll');
  link(files,'f3f','f') ;
end

// Calling ex17f with the Fortran argument function fp3
Z3=ex17f(X,Y,'f3f');
if norm(Z3 - Zref3) > %eps then pause,end


