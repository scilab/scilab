getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/balanc_data.ref','rb');
A = [1/(2^10),1/(2^10);2^10,2^10];
[Ab,X] = balanc(A);
%ans = norm(A(1, :))/norm(A(2, :));
if load_ref('%ans') then   pause,end,

%ans = norm(Ab(1, :))/norm(Ab(2, :));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
