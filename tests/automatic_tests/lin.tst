getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lin_data.ref','rb');
%ans = deff('[y,xdot]=sim(x,u)', 'xdot=[u*sin(x);-u*x^2];y=xdot(1)+xdot(2)');
if load_ref('%ans') then   pause,end,

sl = lin(sim, 1, 2);
xdel_run(winsid());

mclose(%U);
