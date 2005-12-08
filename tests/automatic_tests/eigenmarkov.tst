getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/eigenmarkov_data.ref','rb');
//P has two recurrent classes (with 2 and 1 states) 2 transient states
P = genmarkov([2,1], 2);
if load_ref('P') then   pause,end,

[M,Q] = eigenmarkov(P);
%ans = P * Q - Q;
if load_ref('%ans') then   pause,end,

%ans = Q * M - (P^20);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
