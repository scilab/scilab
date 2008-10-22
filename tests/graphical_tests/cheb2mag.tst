getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/cheb2mag_data.ref','rb');
//Chebyshev; ripple in the stopband
n = 10;omegar = 6;A = 1/0.2;sample = 0.0001:0.05:10;
h2 = cheb2mag(n, omegar, A, sample);
%ans = plot(sample, log(h2)/log(10), 'frequencies', 'magnitude in dB');
if load_ref('%ans') then   pause,end,

//Plotting of frequency edges
minval = (-maxi(-log(h2)))/log(10);
plot2d([omegar;omegar], [minval;0], 2, '000');
//Computation of the attenuation in dB at the stopband edge
attenuation = -log(A * A)/log(10);
%ans = plot2d(sample', attenuation * ones(sample)', 5, '000');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
