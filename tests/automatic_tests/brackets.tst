getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/brackets_data.ref','rb');
%ans = [6.9,9.640000000000001;sqrt(-1),0];
if load_ref('%ans') then   pause,end,

%ans = [1,%i,2,-%i,3];
if load_ref('%ans') then   pause,end,

%ans = [];
if load_ref('%ans') then   pause,end,

%ans = ['this is';'a string';'vector'];
if load_ref('%ans') then   pause,end,

s = poly(0, 's');%ans = [1/s,2/s];
if load_ref('%ans') then   pause,end,

%ans = [tf2ss(1/s),tf2ss(2/s)];
if load_ref('%ans') then   pause,end,


[u,s] = schur(rand(3, 3));
if load_ref('s') then   pause,end,
if load_ref('u') then   pause,end,

xdel_run(winsid());

mclose(%U);
