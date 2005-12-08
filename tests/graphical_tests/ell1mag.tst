getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/ell1mag_data.ref','rb');
%ans = deff('[alpha,beta]=alpha_beta(n,m,m1)', 'if 2*int(n/2)=n then, beta=K1; else, beta=0;end;alpha=%k(1-m1)/%k(1-m);');
if load_ref('%ans') then   pause,end,

epsilon = 0.1;A = 10;//ripple parameters
m1 = epsilon * epsilon/(A * A - 1);n = 5;omegac = 6;
m = find_freq(epsilon, A, n);omegar = omegac/sqrt(m);
if load_ref('omegar') then   pause,end,

%ans = %k(1 - m1) * %k(m)/(%k(m1) * %k(1 - m)) - n;
if load_ref('%ans') then   pause,end,
//Check...
[alpha,beta] = alpha_beta(n, m, m1);
if load_ref('beta') then   pause,end,
if load_ref('alpha') then   pause,end,

%ans = alpha * %asn(1, m) - n * %k(m1);
if load_ref('%ans') then   pause,end,
//Check
sample = 0:0.01:20;
//Now we map the positive real axis into the contour...
z = alpha * %asn(sample/omegac, m) + beta * ones(sample);
%ans = plot(sample, ell1mag(epsilon, m1, z));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
