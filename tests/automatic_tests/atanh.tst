getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/atanh_data.ref','rb');
// example 1
x = [0,%i,-%i];
if load_ref('x') then   pause,end,

%ans = tanh(atanh(x));
if load_ref('%ans') then   pause,end,


// example 2
x = [-%inf,-3,-2,-1,0,1,2,3,%inf];
if load_ref('x') then   pause,end,

%ans = ieee(2);
if load_ref('%ans') then   pause,end,

%ans = atanh(tanh(x));
if load_ref('%ans') then   pause,end,


// example 3 (see Remark)
%ans = ieee(2);
if load_ref('%ans') then   pause,end,

%ans = atanh([1,2]);
if load_ref('%ans') then   pause,end,

%ans = atanh([1,0.5]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
