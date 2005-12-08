getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/dft_data.ref','rb');
n = 8;omega = exp(-2 * %pi * %i/n);
j = 0:n - 1;F = omega.^(j' * j);//Fourier matrix
x = 1:8;x = x(:);
%ans = F * x;
if load_ref('%ans') then   pause,end,

%ans = fft(x, -1);
if load_ref('%ans') then   pause,end,

%ans = dft(x, -1);
if load_ref('%ans') then   pause,end,

%ans = inv(F) * x;
if load_ref('%ans') then   pause,end,

%ans = fft(x, 1);
if load_ref('%ans') then   pause,end,

%ans = dft(x, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
