getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/cheb1mag_data.ref','rb');
//Chebyshev; ripple in the passband
n = 13;epsilon = 0.2;omegac = 3;sample = 0:0.05:10;
h = cheb1mag(n, omegac, epsilon, sample);
%ans = plot(sample, h, 'frequencies', 'magnitude');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
