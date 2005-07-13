getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/graphic/cepstrum_data.ref','r');
w = 0.1:0.1:5;mag = 1 + abs(sin(w));
fresp = cepstrum(w, mag);
%ans = plot2d([w',w'], [mag(:),abs(fresp)]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
