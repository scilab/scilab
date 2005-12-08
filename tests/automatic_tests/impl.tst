getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/impl_data.ref','rb');
y = impl([1;0;0], [-0.04;0.04;0], 0, 0.4, 'resid', 'aplusp');
// Using hot restart
//[x1,w,iw]=impl([1;0;0],[-0.04;0.04;0],0,0.2,'resid','aplusp');
// hot start from previous call
//[x1]=impl([1;0;0],[-0.04;0.04;0],0.2,0.4,'resid','aplusp',w,iw);
//maxi(abs(x1-x))
xdel_run(winsid());

mclose(%U);
