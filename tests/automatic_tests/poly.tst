getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/poly_data.ref','r');
s = poly(0, 's');p = 1 + s + 2 * (s^2);
A = rand(2, 2);%ans = poly(A, 'x');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
