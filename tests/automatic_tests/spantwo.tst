getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/spantwo_data.ref','rb');
A = [1,0,0,4;
  5,6,7,8;
  0,0,11,12;
  0,0,0,16];
B = [1,2,0,0]';C = [4,0,0,1];
Sl = ss2ss(syslin('c', A, B, C), rand(A));
[no,X] = contr(Sl('A'), Sl('B'));CO = X(:, 1:no);//Controllable part
[uo,Y] = unobs(Sl('A'), Sl('C'));UO = Y(:, 1:uo);//Unobservable part
[Xp,dimc,dimu,dim] = spantwo(CO, UO);//Kalman decomposition
Slcan = ss2ss(Sl, inv(Xp));
xdel_run(winsid());

mclose(%U);
