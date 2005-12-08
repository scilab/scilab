getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/analpf_data.ref','rb');
//Evaluate magnitude response of continuous-time system
hs = analpf(4, 'cheb1', [0.1,0], 5);
if load_ref('hs') then   pause,end,

fr = 0:0.1:15;
hf = freq(hs(2), hs(3), %i * fr);
hm = abs(hf);
%ans = plot(fr, hm);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
