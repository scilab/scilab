getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ctr_gram_data.ref','rb');
A = diag([-1,-2,-3]);B = rand(3, 2);
Wc = ctr_gram(A, B);
if load_ref('Wc') then   pause,end,

U = rand(3, 3);A1 = U * A/U;B1 = U * B;
Wc1 = ctr_gram(A1, B1);
if load_ref('Wc1') then   pause,end,
//Not invariant!
xdel_run(winsid());

mclose(%U);
