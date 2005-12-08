getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lsqrsolve_data.ref','rb');
// A simple example with lsqrsolve
a = [1,7;
  2,8;
  4,3];
b = [10;11;-1];
function y=f1(x,m)
  ,y=a*x+b;
endfunction
if load_ref('%ans') then   pause,end,

[xsol,v] = lsqrsolve([100;100], f1, 3);
if load_ref('v') then   pause,end,
if load_ref('xsol') then   pause,end,

%ans = xsol + a\b;
if load_ref('%ans') then   pause,end,



function y=fj1(x,m)
  ,y=a;
endfunction
if load_ref('%ans') then   pause,end,

[xsol,v] = lsqrsolve([100;100], f1, 3, fj1);
if load_ref('v') then   pause,end,
if load_ref('xsol') then   pause,end,

%ans = xsol + a\b;
if load_ref('%ans') then   pause,end,


// Data fitting problem
// 1 build the data
a = 34;b = 12;c = 14;
deff('y=FF(x)', 'y=a*(x-b)+c*x.*x');
X = (0:0.1:3)';Y = FF(X) + 100 * (rand() - 0.5);

//solve
function e=f1(abc,m)
    a=abc(1);b=abc(2),c=abc(3),
    e=Y-(a*(X-b)+c*X.*X);
endfunction
if load_ref('%ans') then   pause,end,

[abc,v] = lsqrsolve([10;10;10], f1, size(X, 1));
abc;
if load_ref('abc') then   pause,end,

%ans = norm(v);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
