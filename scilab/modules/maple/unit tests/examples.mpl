# load macrofort to be able o generate Fortran code
read "macrofort.mpl":
# load macroC to be able to generate C code
read "macroC.mpl":
# load maple2scilab
read "maple2scilab.mpl":

# If necessary, change the path of Scilab below
machine_include:="/usr/local/lib/scilab/routines/machine.h":

# we want optimized code
optimized:=true:

# If necessary, change the way to call Fortran compiler below
compF:=proc(file) local FF;
FF:='f77';
system(``||FF||` -c `||file||`.f`);
end:

# If necessary, change the way to call C compiler below
compC:=proc(file) local CC;
CC:='cc';
system(``||CC||` -c `||file||`.c`);
end:

################################################
# example 1: algebraic expression
################################################

m:=exp(1+x)/sin(x);

maple2scilab(foof1,m,x,'f');
compF(foof1);

maple2scilab(fooc1,m,x,'c');
compC(fooc1);

maple2scilab(foos1,m,x,'s');

################################################
# example 2: numerical matrix without parameter
################################################

m:=linalg[randmatrix](5,10);

maple2scilab(foof2,m,[],'f');
compF(foof2);

maple2scilab(fooc2,m,[],'c');
compC(fooc2);

maple2scilab(foos2,m,[],'s');

################################################
# example 3: vector with scalar parameter x
################################################

v:=vector([sqrt(x),sin(1-x^2)]);

maple2scilab(foof3,v,[x],'f');
compF(foof3);

maple2scilab(fooc3,v,[x],'c');
compC(fooc3);

maple2scilab(foos3,v,[x],'s');

################################################
# example 4: matrix with scalar parameters x and y
################################################

m:=matrix([[x,y]]);

maple2scilab(foof4,m,[x,y],'f');
compF(foof4);

maple2scilab(fooc4,m,[x,y],'c');
compC(fooc4);

maple2scilab(foos4,m,[x,y],'s');

################################################
# example 5: matrix with a scalar parameter k and a vector parameter x
################################################

x:=vector(3);
m:=matrix([[k*x[1],x[2]-x[1]],[(1-k)*sin(x[3]),x[3]]]);

maple2scilab(foof5,m,[k,x],'f');
compF(foof5);

maple2scilab(fooc5,m,[k,x],'c');
compC(fooc5);

maple2scilab(foos5,m,[k,x],'s');

################################################
# example 6: matrix with a scalar parameter k and a matrix parameter x
################################################

x:=matrix(2,3);
m:=matrix([[x[1,1]+x[2,2],k*(x[1,2]+x[2,1])],[10*k,1],[x[2,3],x[1,3]]]);

maple2scilab(foof6,m,[k,x],'f');
compF(foof6);

maple2scilab(fooc6,m,[k,x],'c');
compC(fooc6);

maple2scilab(foos6,m,[k,x],'s');

################################################
# example 7: sparse matrix with a scalar parameter k and a matrix parameter x
################################################

x:=matrix(2,3);
m:=array(sparse,1..20,1..20);
m[1,1]:=x[1,1]+x[2,2];
m[2,4]:=k*x[2,3];
m[15,6]:=k*(x[1,2]-x[2,1]);

maple2scilab(foof7,m,[k,x],'f');
compF(foof7);

maple2scilab(fooc7,m,[k,x],'c');
compC(fooc7);

maple2scilab(foos7,m,[k,x],'s');

################################################
# example 8: same names for Maple matrix and Scilab function
################################################

foof8:=linalg[randmatrix](5,10);
fooc8:=linalg[randmatrix](5,10):
foos8:=linalg[randmatrix](5,10):

maple2scilab('foof8',foof8,[],'f');
compF(foof8);

maple2scilab('fooc8',fooc8,[],'c');
compC(fooc8);

maple2scilab('foos8',foos8,[],'s');

################################################
# example 9: problem when we cut at a point for Scilab code generation
################################################

v:=vector([xxxxxxx*expand((1+x)^8)+123.456,xxx*expand((1+x)^8)+12345.]);

maple2scilab('foos9',v,[x,xxx,xxxxxxx],'s');

quit
