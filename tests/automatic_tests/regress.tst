getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/regress_data.ref','rb');
x = [0.5608486,0.6623569,0.7263507,0.1985144,0.5442573,0.2320748,0.2312237];
if load_ref('x') then   pause,end,

y = [0.3616361,0.2922267,0.5664249,0.4826472,0.3321719,0.5935095,0.5015342];
if load_ref('y') then   pause,end,

coefs = regress(x, y);
if load_ref('coefs') then   pause,end,

xdel_run(winsid());

mclose(%U);
