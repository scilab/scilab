getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pvm_f772sci_data.ref','rb');
a = [1 + %i,2 + 2 * %i,3 + 3 * %i];
%ans = pvm_f772sci(a);
if load_ref('%ans') then   pause,end,

if norm(a - [1 + 2 * %i,3 + %i,2 + 3 * %i]) > 10 * %eps then   pause,end,
xdel_run(winsid());

mclose(%U);
