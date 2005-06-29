getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/int8_data.ref','r');
%ans = int8([1,-120,127,312]);
if load_ref('%ans') then   pause,end,

%ans = uint8([1,-120,127,312]);
if load_ref('%ans') then   pause,end,


x = int32(-200:100:400);
if load_ref('x') then   pause,end,

%ans = int8(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
