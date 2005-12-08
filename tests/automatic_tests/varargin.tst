getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/varargin_data.ref','rb');
%ans = deff('exampl(a,varargin)', ['[lhs,rhs]=argn(0)';
  'if rhs>=1 then disp(varargin),end']);
if load_ref('%ans') then   pause,end,

%ans = exampl(1);
if load_ref('%ans') then   pause,end,

%ans = exampl();
if load_ref('%ans') then   pause,end,

%ans = exampl(1, 2, 3);
if load_ref('%ans') then   pause,end,

l = list('a', %s, %t);
%ans = exampl(1, l(2:3));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
