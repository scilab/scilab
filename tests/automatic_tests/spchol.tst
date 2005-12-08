getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/spchol_data.ref','rb');

X = [3,0,0,2,0,0,2,0,2,0,0;
  0,5,4,0,0,0,0,0,0,0,0;
  0,4,5,0,0,0,0,0,0,0,0;
  2,0,0,3,0,0,2,0,2,0,0;
  0,0,0,0,5,0,0,0,0,0,4;
  0,0,0,0,0,4,0,3,0,3,0;
  2,0,0,2,0,0,3,0,2,0,0;
  0,0,0,0,0,3,0,4,0,3,0;
  2,0,0,2,0,0,2,0,3,0,0;
  0,0,0,0,0,3,0,3,0,4,0;
  0,0,0,0,4,0,0,0,0,0,5];
X = sparse(X);[R,P] = spchol(X);
%ans = max(P * R * R' * P' - X);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
