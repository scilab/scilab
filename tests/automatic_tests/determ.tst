getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/determ_data.ref','rb');
s = poly(0, 's');
w = s * rand(10, 10);
%ans = determ(w);
if load_ref('%ans') then   pause,end,

%ans = det(coeff(w, 1)) * (s^10);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
