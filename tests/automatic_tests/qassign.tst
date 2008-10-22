getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/qassign_data.ref','rb');
n = 15;
d = 100 * rand(15, 15);
d = d - diag(diag(d));
c = zeros(n, n);f = c;
f(2:n, 1) = ones(1:n - 1)';
[crit,order] = qassign(c, f, d);
if load_ref('order') then   pause,end,
if load_ref('crit') then   pause,end,

xdel_run(winsid());

mclose(%U);
