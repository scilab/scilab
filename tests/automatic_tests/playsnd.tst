getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/playsnd_data.ref','r');
y = wavread('SCI/demos/signal/sound/chimes.wav');
%ans = playsnd(y);
if load_ref('%ans') then   pause,end,



xdel_run(winsid());

mclose(%U);
