getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/plzr_data.ref','rb');
s = poly(0, 's');
n = [1 + s,2 + 3 * s + 4 * (s^2),5;0,1 - s,s];
d = [1 + 3 * s,5 - (s^3),s + 1;1 + s,1 + s + s^2,3 * s - 1];
h = syslin('c', n ./ d);
plzr(h);
xdel_run(winsid());

mclose(%U);
