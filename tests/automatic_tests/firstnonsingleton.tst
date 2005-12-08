getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/firstnonsingleton_data.ref','rb');
A = [1,2,3;4,5,6];
// Scilab max
M = max(A);
if load_ref('M') then   pause,end,

// Matlab max emulation
M = max(A, firstnonsingleton(A));
if load_ref('M') then   pause,end,

xdel_run(winsid());

mclose(%U);
