getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/hat_data.ref','r');
%ans = 2^4;
if load_ref('%ans') then   pause,end,

%ans = [1,2;2,4]^(1 + %i);
if load_ref('%ans') then   pause,end,

s = poly(0, 's');
%ans = [1,2,s]^4;
if load_ref('%ans') then   pause,end,

%ans = [s,1;1,s]^(-1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
