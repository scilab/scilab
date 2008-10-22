getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/atan_data.ref','rb');
// examples with the second form
x = [1,%i,-1,%i];
if load_ref('x') then   pause,end,

phasex = atan(imag(x), real(x));
if load_ref('phasex') then   pause,end,

%ans = atan(0, -1);
if load_ref('%ans') then   pause,end,

%ans = atan(-%eps, -1);
if load_ref('%ans') then   pause,end,


// branch cuts
%ans = atan(-%eps + 2 * %i);
if load_ref('%ans') then   pause,end,

%ans = atan(%eps + 2 * %i);
if load_ref('%ans') then   pause,end,

%ans = atan(-%eps - 2 * %i);
if load_ref('%ans') then   pause,end,

%ans = atan(%eps - 2 * %i);
if load_ref('%ans') then   pause,end,


// values at the branching points
%ans = ieee(2);
if load_ref('%ans') then   pause,end,

%ans = atan(%i);
if load_ref('%ans') then   pause,end,

%ans = atan(-%i);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
