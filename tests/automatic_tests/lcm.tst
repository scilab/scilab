getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lcm_data.ref','rb');
//polynomial case
s = poly(0, 's');
p = [s,s * ((s + 1)^2),(s^2) * (s + 2)];
[pp,fact] = lcm(p);
%ans = p .* fact;
if load_ref('%ans') then   pause,end,
pp;
if load_ref('pp') then   pause,end,

//integer case
V = int32([(2^2) * (3^5),(2^3) * (3^2),(2^2) * (3^4) * 5]);
%ans = lcm(V);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
