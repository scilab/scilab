getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ftuneq_data.ref','rb');
samples = [46,55,54;53,54,50;49,58,51;50,61,51;46,52,49];
if load_ref('samples') then   pause,end,

[f,p] = ftest(samples);
if load_ref('p') then   pause,end,
if load_ref('f') then   pause,end,

xdel_run(winsid());

mclose(%U);
