getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/graphic/hilb_data.ref','r');
%ans = plot(hilb(51));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
