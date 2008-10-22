getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/armac_data.ref','rb');
a = [1,-2.851,2.717,-0.865] .*. eye(2, 2);
if load_ref('a') then   pause,end,

b = [0,1,1,1] .*. [1;1];
d = [1,0.7,0.2] .*. eye(2, 2);
sig = eye(2, 2);
ar = armac(a, b, d, 2, 1, sig);
if load_ref('ar') then   pause,end,

// extract polynomial matrices from ar representation
[A,B,D] = arma2p(ar);
xdel_run(winsid());

mclose(%U);
