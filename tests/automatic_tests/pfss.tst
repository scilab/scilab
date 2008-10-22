getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pfss_data.ref','rb');
W = ssrand(1, 1, 6);
elts = pfss(W);
W1 = 0;for k = 1:size(elts),  W1 = W1 + ss2tf(elts(k));end,
%ans = clean(ss2tf(W) - W1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
