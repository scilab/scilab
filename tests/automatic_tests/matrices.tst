getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/matrices_data.ref','rb');
E = [1,2;3,4];
if load_ref('E') then   pause,end,

E = [%T,%F;1 == 1,1 ~= 1];
if load_ref('E') then   pause,end,

s = poly(0, 's');E = [s,s^2;1,1 + s];
if load_ref('E') then   pause,end,

E = [1/s,0;s,1/(s + 1)];
if load_ref('E') then   pause,end,

E = ['A11','A12';'A21','A22'];
if load_ref('E') then   pause,end,

xdel_run(winsid());

mclose(%U);
