getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/cont_frm_data.ref','rb');
s = poly(0, 's');NUM = [1 + s,s];den = s^2 - 5 * s + 1;
sl = cont_frm(NUM, den);
slss = ss2tf(sl);//Compare with NUM/den
xdel_run(winsid());

mclose(%U);
