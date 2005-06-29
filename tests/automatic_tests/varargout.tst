getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/varargout_data.ref','r');
%ans = deff('varargout=exampl()', 'varargout=list(1,2,3,4)');
if load_ref('%ans') then   pause,end,


x = exampl();
if load_ref('x') then   pause,end,

[x,y] = exampl();
if load_ref('y') then   pause,end,
if load_ref('x') then   pause,end,

[x,y,z] = exampl();
if load_ref('z') then   pause,end,
if load_ref('y') then   pause,end,
if load_ref('x') then   pause,end,

xdel_run(winsid());

mclose(%U);
