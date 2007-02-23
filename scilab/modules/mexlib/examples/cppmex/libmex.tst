// load the shared library 

exec loader.sce ;

// run tests 

x=56;
y = square(x);
if norm(y-x^2) > %eps then pause,end




