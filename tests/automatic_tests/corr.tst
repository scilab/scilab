getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/corr_data.ref','rb');
x = %pi/10:%pi/10:102.4 * %pi;
rand('seed');rand('normal');
y = [0.8 * sin(x) + 0.8 * sin(2 * x) + rand(x);0.8 * sin(x) + 0.8 * sin(1.99 * x) + rand(x)];
c = [];
for j = 1:2,  for k = 1:2,  c = [c;corr(y(k, :), y(j, :), 64)];end,end,
c = matrix(c, 2, 128);cov = [];
for j = 1:64,  cov = [cov;c(:, (j - 1) * 2 + 1:2 * j)];end,
%ans = rand('unif');
if load_ref('%ans') then   pause,end,

//
rand('normal');x = rand(1, 256);y = -x;
deff('[z]=xx(inc,is)', 'z=x(is:is+inc-1)');
deff('[z]=yy(inc,is)', 'z=y(is:is+inc-1)');
[c,mxy] = corr(x, y, 32);
x = x - mxy(1) * ones(x);y = y - mxy(2) * ones(y);//centring
c1 = corr(x, y, 32);c2 = corr(x, 32);
%ans = norm(c1 + c2, 1);
if load_ref('%ans') then   pause,end,

[c3,m3] = corr('fft', xx, yy, 256, 32);
%ans = norm(c1 - c3, 1);
if load_ref('%ans') then   pause,end,

[c4,m4] = corr('fft', xx, 256, 32);
%ans = norm(m3, 1);
if load_ref('%ans') then   pause,end,
%ans = norm(m4, 1);
if load_ref('%ans') then   pause,end,

%ans = norm(c3 - c1, 1);
if load_ref('%ans') then   pause,end,
%ans = norm(c4 - c2, 1);
if load_ref('%ans') then   pause,end,

x1 = x(1:128);x2 = x(129:256);
y1 = y(1:128);y2 = y(129:256);
w0 = 0 * ones(1:64);//32 coeffs
[w1,xu] = corr('u', x1, y1, w0);w2 = corr('u', x2, y2, w1, xu);
zz = real(fft(w2, 1))/256;c5 = zz(1:32);
%ans = norm(c5 - c1, 1);
if load_ref('%ans') then   pause,end,

[w1,xu] = corr('u', x1, w0);w2 = corr('u', x2, w1, xu);
zz = real(fft(w2, 1))/256;c6 = zz(1:32);
%ans = norm(c6 - c2, 1);
if load_ref('%ans') then   pause,end,

%ans = rand('unif');
if load_ref('%ans') then   pause,end,

// test for Fortran or C external
//
deff('[y]=xmacro(sec,ist)', 'y=sin(ist:(ist+sec-1))');
x = xmacro(100, 1);
[cc1,mm1] = corr(x, 2^3);
[cc,mm] = corr('fft', xmacro, 100, 2^3);
[cc2,mm2] = corr('fft', 'corexx', 100, 2^3);
%ans = [maxi(abs(cc - cc1)),maxi(abs(mm - mm1)),maxi(abs(cc - cc2)),maxi(abs(mm - mm2))];
if load_ref('%ans') then   pause,end,


deff('[y]=ymacro(sec,ist)', 'y=cos(ist:(ist+sec-1))');
y = ymacro(100, 1);
[cc1,mm1] = corr(x, y, 2^3);
[cc,mm] = corr('fft', xmacro, ymacro, 100, 2^3);
[cc2,mm2] = corr('fft', 'corexx', 'corexy', 100, 2^3);
%ans = [maxi(abs(cc - cc1)),maxi(abs(mm - mm1)),maxi(abs(cc - cc2)),maxi(abs(mm - mm2))];
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
