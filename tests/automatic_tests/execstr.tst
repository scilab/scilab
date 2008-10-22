getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/execstr_data.ref','rb');
%ans = execstr('a=1');
if load_ref('%ans') then   pause,end,
// sets a=1.
%ans = execstr('1+1');
if load_ref('%ans') then   pause,end,
// does nothing (while evstr('1+1') returns 2)

%ans = execstr(['if %t then';
  '  a=1';
  '  b=a+1';
  'else';
  ' b=0';
  'end']);
if load_ref('%ans') then   pause,end,


%ans = execstr('a=zzzzzzz', 'errcatch');
if load_ref('%ans') then   pause,end,

%ans = execstr('a=zzzzzzz', 'errcatch', 'm');
if load_ref('%ans') then   pause,end,


//syntax errors
%ans = execstr('a=1?02', 'errcatch');
if load_ref('%ans') then   pause,end,

%ans = lasterror(%t);
if load_ref('%ans') then   pause,end,


%ans = execstr('a=[1 2 3)', 'errcatch');
if load_ref('%ans') then   pause,end,

%ans = lasterror(%t);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
