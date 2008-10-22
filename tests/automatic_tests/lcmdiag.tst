getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lcmdiag_data.ref','rb');
s = poly(0, 's');
H = [1/s,(s + 2)/s/((s + 1)^2);1/((s^2) * (s + 2)),2/(s + 2)];
[N,D] = lcmdiag(H);
%ans = N/D - H;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
