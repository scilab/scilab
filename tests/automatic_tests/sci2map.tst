getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/sci2map_data.ref','r');
txt = [sci2map([1,2;3,4], 'a');
  sci2map(%s^2 + 3 * %s + 4, 'p')];
if load_ref('txt') then   pause,end,

xdel_run(winsid());

mclose(%U);
