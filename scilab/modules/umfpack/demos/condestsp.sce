// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Using small matrices to test condestsp

function demo_condestsp()

    nb_tests = 20;
    n = 100;
    K1 = zeros(nb_tests,3);

    msg = _("A small test for the condestsp function: condestsp gives an estimate of\n the condition number K1 in 1-norm of a real sparse matrix A:")
    mprintf(msg)
    mprintf("     K1 = ||A||_1 * ||A^(-1)||_1  ")
    msg = _("     without explicitly computing the inverse of A. condestsp uses a factorization given by\n umf_lufact but if you have already computed this one it is recommended to give the pointer to the factorization.")
    mprintf(msg)
    msg = _("The test consists in forming small sparse matrices (so as to compute K1 exactly with\nnorm(inv(full(A)),1)) whose values are chosen from the normal distribution.")
    mprintf(msg)
    halt(_("\nPress Return to continue...\n"))

    for k = 1:nb_tests
        m = grand(1,1,"uin",1000,2000);  // nnz
        ij = grand(m,2,"uin",1,100);
        v = grand(m,1,"nor",0,1);
        A = sparse(ij,v,[n n]);
        Lup = umf_lufact(A);
        K1(k,1) = condestsp(A,Lup);
        K1(k,2) = condestsp(A,Lup,5);
        K1(k,3) = norm(A,1)*norm(inv(full(A)),1);
        umf_ludel(Lup)
    end

    str2 = "------------";
    str3 = "condest  t=2";
    str4 = "condest  t=5";
    str5 = "  K1 exact  ";

    mprintf(" +-%s-+-%s-+-%s-+ \n",str2,str2,str2)
    mprintf(" | %s | %s | %s | \n",str3,str4,str5)
    mprintf(" +-%s-+-%s-+-%s-+ \n",str2,str2,str2)
    for k=1:nb_tests
        mprintf(" | %e | %e | %e | \n",K1(k,1:3))
    end
    mprintf(" +-%s-+-%s-+-%s-+ \n",str2,str2,str2)

endfunction

demo_condestsp()
clear demo_condestsp;
