getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/grep_data.ref','r');
txt = ['find matches of a string in a vector of strings';
  'search position of a character string in an other string';
  'Compare Strings'];

%ans = grep(txt, 'strings');
if load_ref('%ans') then   pause,end,

%ans = grep(txt, ['strings','Strings']);
if load_ref('%ans') then   pause,end,


[r,w] = grep(txt, ['strings','Strings']);
if load_ref('w') then   pause,end,
if load_ref('r') then   pause,end,


xdel_run(winsid());

mclose(%U);
