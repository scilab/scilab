getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/roots_data.ref','rb');
p = poly([0,10,1 + %i,1 - %i], 'x');
%ans = roots(p);
if load_ref('%ans') then   pause,end,

A = rand(3, 3);%ans = roots(poly(A, 'x'));
if load_ref('%ans') then   pause,end,
// Evals by characteristic polynomial
%ans = spec(A);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
