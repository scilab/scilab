getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/schur_data.ref','rb');
//SCHUR FORM OF A MATRIX
//----------------------
A = diag([-0.9,-2,2,0.9]);X = rand(A);A = inv(X) * A * X;
[U,T] = schur(A);T;
if load_ref('T') then   pause,end,


[U,dim,T] = schur(A, 'c');
%ans = T(1:dim, 1:dim);
if load_ref('%ans') then   pause,end,
//stable cont. eigenvalues

function t=mytest(Ev)
  ,t=abs(Ev)<0.95,
endfunction
if load_ref('%ans') then   pause,end,

[U,dim,T] = schur(A, mytest);
%ans = T(1:dim, 1:dim);
if load_ref('%ans') then   pause,end,


// The same function in C (a Compiler is required)
C = ['int mytest(double *EvR, double *EvI) {'; //the C code;
  'if (*EvR * *EvR + *EvI * *EvI < 0.9025) return 1;';
  'else return 0; }'];
if load_ref('C') then   pause,end,

%ans = mputl(C, TMPDIR + '/mytest.c');
if load_ref('%ans') then   pause,end,



//build and link
lp = ilib_for_link('mytest', 'mytest.o', [], 'c', TMPDIR + '/Makefile');
link(lp, 'mytest', 'c');

//run it
[U,dim,T] = schur(A, 'mytest');
//SCHUR FORM OF A PENCIL
//----------------------
F = [-1,%s,0,1;
  0,-1,5 - %s,0;
  0,0,2 + %s,0;
  1,0,0,-2 + %s];
A = coeff(F, 0);E = coeff(F, 1);
[As,Es,Q,Z] = schur(A, E);
%ans = Q' * F * Z;
if load_ref('%ans') then   pause,end,
//It is As+%s*Es


[As,Es,Z,dim] = schur(A, E, 'c');
if load_ref('dim') then   pause,end,
if load_ref('Z') then   pause,end,
if load_ref('Es') then   pause,end,
if load_ref('As') then   pause,end,

function t=mytest(Alpha,Beta)
  ,t=real(Alpha)<0,
endfunction
if load_ref('%ans') then   pause,end,

[As,Es,Z,dim] = schur(A, E, mytest);
if load_ref('dim') then   pause,end,
if load_ref('Z') then   pause,end,
if load_ref('Es') then   pause,end,
if load_ref('As') then   pause,end,


//the same function in Fortran (a Compiler is required)
ftn = ['integer function mytestf(ar,ai,b)'; //the fortran code;
  'double precision ar,ai,b';
  'mytestf=0';
  'if(ar.lt.0.0d0) mytestf=1';
  'end'];
if load_ref('ftn') then   pause,end,

%ans = mputl('      ' + ftn, TMPDIR + '/mytestf.f');
if load_ref('%ans') then   pause,end,


//build and link
lp = ilib_for_link('mytestf', 'mytestf.o', [], 'F', TMPDIR + '/Makefile');
link(lp, 'mytestf', 'f');

//run it

[As,Es,Z,dim] = schur(A, E, 'mytestf');
if load_ref('dim') then   pause,end,
if load_ref('Z') then   pause,end,
if load_ref('Es') then   pause,end,
if load_ref('As') then   pause,end,

xdel_run(winsid());

mclose(%U);
