getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/trisolve_data.ref','rb');
A = ['x','y';'0','z'];b = ['0';'1'];
w = trisolve(A, b);
if load_ref('w') then   pause,end,

x = 5;y = 2;z = 4;
%ans = evstr(w);
if load_ref('%ans') then   pause,end,

%ans = inv(evstr(A)) * evstr(b);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
