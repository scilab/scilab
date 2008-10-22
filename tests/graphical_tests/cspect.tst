getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/cspect_data.ref','rb');
rand('normal');rand('seed', 0);
x = rand(1:1024 - 33 + 1);
//make low-pass filter with eqfir
nf = 33;bedge = [0,0.1;0.125,0.5];des = [1,0];wate = [1,1];
h = eqfir(nf, bedge, des, wate);
//filter white data to obtain colored data
h1 = [h,0 * ones(1:maxi(size(x)) - 1)];
x1 = [x,0 * ones(1:maxi(size(h)) - 1)];
hf = fft(h1, -1);xf = fft(x1, -1);yf = hf .* xf;y = real(fft(yf, 1));
sm = cspect(100, 200, 'tr', y);
smsize = maxi(size(sm));fr = (1:smsize)/smsize;
%ans = plot(fr, log(sm));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
