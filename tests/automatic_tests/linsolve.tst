getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/linsolve_data.ref','rb');
A = rand(5, 3) * rand(3, 8);
b = A * ones(8, 1);[x,kerA] = linsolve(A, b);%ans = A * x + b;
if load_ref('%ans') then   pause,end,
//compatible b
b = ones(5, 1);[x,kerA] = linsolve(A, b);%ans = A * x + b;
if load_ref('%ans') then   pause,end,
//uncompatible b
A = rand(5, 5);[x,kerA] = linsolve(A, b);
if load_ref('kerA') then   pause,end,
if load_ref('x') then   pause,end,
%ans = -inv(A) * b;
if load_ref('%ans') then   pause,end,
//x is unique
xdel_run(winsid());

mclose(%U);
