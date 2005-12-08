getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/inv_coeff_data.ref','rb');
A = int(10 * rand(2, 6));
if load_ref('A') then   pause,end,

// Building a degree 1 polynomial matrix
P = inv_coeff(A, 1);
if load_ref('P') then   pause,end,

%ans = norm(coeff(P) - A);
if load_ref('%ans') then   pause,end,

// Using default value for degree
P1 = inv_coeff(A);
if load_ref('P1') then   pause,end,

%ans = norm(coeff(P1) - A);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
