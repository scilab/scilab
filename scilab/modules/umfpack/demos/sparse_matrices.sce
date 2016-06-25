// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// The simple demo for the umfpack interface & others
// sparse utility (scispt toolbox)

function demo_sparse_matrices()

    format(15);
    cd "SCI/modules/umfpack/demos/"

    disp(_("To load a sparse matrix stored in the Harwell-Boeing format in Scilab, you may use the function ReadHBSparse."));
    disp(_("4 matrices will be loaded for the demo: they come from the University of Florida Sparse Matrix Collection:"));
    printf("     www.cise.ufl.edu/research/sparse/matrices\n"+..
    "     http://math.nist.gov/MatrixMarket/matrices.html");
    disp(_("    maintained by Tim Davis, author of UMFPACK"));

    mode(1)
    [A1,descr1,ref1,mtype1] = ReadHBSparse("arc130.rua");
    [A2,descr2,ref2,mtype2] = ReadHBSparse("bcsstk24.rsa");
    [A3,descr3,ref3,mtype3] = ReadHBSparse("ex14.rua");
    [A4,descr4,ref4,mtype4] = ReadHBSparse("young1c.csa");
    mode(-1)

    disp(_("To see the pattern of non zeros elements, you may use the function PlotSparse."));
    disp(_("Here we will display the 4 matrices"));
    halt(_("\nPress Return to continue...\n"));

    mode(1)
    g = figure("visible", "off", "backgroundcolor", [1,1,1]);
    old_winsid = winsid();
    num = max(old_winsid);
    g.figure_size = [800 800];
    subplot(221)
    PlotSparse(A1,"y+")
    xtitle(ref1+"."+mtype1+": "+descr1)

    subplot(222)
    PlotSparse(A2)
    xtitle(ref2+"."+mtype2+": "+descr2)

    subplot(223)
    PlotSparse(A3,"c.")
    xtitle(ref3+"."+mtype3+": "+descr3)

    subplot(224)
    PlotSparse(A4,"r.")
    xtitle(ref4+"."+mtype4+": "+descr4)
    set(g, "visible", "on");
    mode(-1)

    disp(_("Now, using umfpack to solve some linear systems:"));
    disp("     b1 = rand(size(A1,1),1);    "+_("-> to create a rhs"));
    disp("     x1 = umfpack(A1,''\'',b1);    "+_("-> to solve A1*x1 = b1"));
    disp("     norm_res = norm(A1*x1-b1);  "+_("-> norm of the residual"));
    disp(_("this is done for the 4 matrices A1, A2, A3 and A4."));
    halt(_("\nPress Return to continue...\n"));

    mode(1)
    n1 = size(A1,1);
    b1 = rand(n1,1);
    tic(); x1 = umfpack(A1,"\",b1); t1=toc();
    norm_res1 = norm(A1*x1-b1);

    n2 = size(A2,1);
    b2 = rand(n2,1);
    tic(); x2 = umfpack(A2,"\",b2); t2=toc();
    norm_res2 = norm(A2*x2-b2);

    n3 = size(A3,1);
    b3 = rand(n3,1);
    tic(); x3 = umfpack(A3,"\",b3); t3=toc();
    norm_res3 =  norm(A3*x3-b3);

    n4 = size(A4,1);
    b4 = rand(n4,1);
    tic(); x4 = umfpack(A4,"\",b4); t4=toc();
    norm_res4 = norm(A4*x4-b4);
    mode(-1)

    disp(_("A1 ("+descr1+"): order = "+ string(n1) + " nnz = " + string(nnz(A1))));
    msgNorm = _("     norm of the residual = ")
    msgCT   = _("     computing time       = ")
    printf(msgNorm + string(norm_res1) + "\n" + msgCT+string(t1)+"\n")
    disp(_("A2 ("+descr2+"): order = "+ string(n2) + " nnz = " + string(nnz(A2))))
    printf(msgNorm + string(norm_res2) + "\n" + msgCT + string(t2) + "\n")
    disp(_("A3 ("+descr3+"): order = "+ string(n3) + " nnz = " + string(nnz(A3))))
    printf(msgNorm + string(norm_res3) + "\n" + msgCT + string(t3)+"\n")
    disp(_("A4 ("+descr4+"): order = "+ string(n4) + " nnz = " + string(nnz(A4))));
    printf(msgNorm + string(norm_res4)+"\n" + msgCT + string(t4)+"\n\n")

    disp(_("Now we will see how to use the lu factors:"));
    disp(_("  1/ lu factors of a sparse matrix A are obtained through:"));
    printf(_("     lup = umf_lufact(A)\n     lup is a pointer to the lu factors (the memory is outside scilab space)\n"))
    disp(_("  2/ for solving a linear system A*x = b, use:"));
    printf(_("     x = umf_lusolve(lup,b)\n     or  x = umf_lusolve(lup,b,""Ax=b"",A)  to add an iterative refinement step\n"))
    disp(_("  3/ to solve A''*x=b you may use:"));
    printf(_("     x = umf_lusolve(lup,b,""Ax''''=b"")\n     or  x = umf_lusolve(lup,b,""Ax''''=b"",A)  to add an iterative refinement step\n"))
    disp(_("  4/ you may also compute the 1-norm condition number quicky with:"));
    printf(_("     K1 = condestsp(A,lup)\n     K1 = condestsp(A) also works but in this case the lu factors are re-computed inside\n"))
    disp(_("  5/ if you don''t need the lu factors anymore, it is recommended to free them with:"));
    printf(_("     umf_ludel(lup)\n     if you have lost your pointer you may use umf_ludel() which frees all the current umf lu factors\n"))
    halt(_("\nPress Return to continue...\n"));

    mode(1)
    lup1 = umf_lufact(A1);
    x1 = umf_lusolve(lup1,b1);
    norm_res1 =  norm(A1*x1-b1);
    x1 = umf_lusolve(lup1,b1,"Ax=b",A1);
    norm_res1r = norm(A1*x1-b1);
    K1 = condestsp(A1,lup1);
    umf_ludel(lup1);

    lup2 = umf_lufact(A2);
    x2 = umf_lusolve(lup2,b2);
    norm_res2 = norm(A2*x2-b2);
    x2 = umf_lusolve(lup2,b2,"Ax=b",A2);
    norm_res2r = norm(A2*x2-b2);
    K2 = condestsp(A2,lup2);
    umf_ludel(lup2);

    lup3 = umf_lufact(A3);
    x3 = umf_lusolve(lup3,b3);
    norm_res3 = norm(A3*x3-b3);
    x3 = umf_lusolve(lup3,b3,"Ax=b",A3);
    norm_res3r = norm(A3*x3-b3);
    K3 = condestsp(A3,lup3);
    umf_ludel(lup3);

    lup4 = umf_lufact(A4);
    x4 = umf_lusolve(lup4,b4);
    norm_res4 = norm(A4*x4-b4);
    x4 = umf_lusolve(lup4,b4,"Ax=b",A4);
    norm_res4r = norm(A4*x4-b4);
    K4 = condestsp(A4,lup4,5);
    umf_ludel(lup4);
    mode(-1)

    disp("A1 ("+descr1+"): order = "+ string(n1) + " nnz = " + string(nnz(A1)))
    printf("     K1 = " + string(K1)+"\n" + ..
         _("     norm of the residual    = ") + string(norm_res1) + "\n" + ..
         _("     same but with refinment = ") + string(norm_res1r) + "\n")
    disp("A2 ("+descr2+"): order = "+ string(n2) + " nnz = " + string(nnz(A2)))
    printf("     K2 = " + string(K2)+"\n"+..
         _("     norm of the residual    = ") + string(norm_res2) + "\n" + ..
         _("     same but with refinment = ") + string(norm_res2r) + "\n")
    disp("A3 ("+descr3+"): order = "+ string(n3) + " nnz = " + string(nnz(A3)))
    printf("     K3 = " + string(K3)+"\n"+..
         _("     norm of the residual    = ") + string(norm_res3) + "\n" + ..
         _("     same but with refinment = ") + string(norm_res3r) + "\n")
    disp("A4 ("+descr4+"): order = "+ string(n4) + " nnz = " + string(nnz(A4)))
    printf("     K4 = " + string(K4) + "\n" + ..
         _("     norm of the residual     = ") + string(norm_res4) + "\n" + ..
         _("     same but with refinement = ") + string(norm_res4r) + "\n\n")

    clear A1 A3 A4 x1 x3 x4 b1 b3 b4

    disp(_("Now we will see how to use the taucs snmf stuff on the matrix A2."));
    disp(_("This is useful and recommended when your matrix is symmetric positive definite (s.p.d.)."));
    disp(_("  1/ the Cholesky factorization of a s.p.d. matrix A is obtained with:"));
    printf("     Cp = taucs_chfact(A)\n" + ..
         _("     Cp is a pointer to the Cholesky fact. (the memory is outside scilab space)\n"));
    disp(_("  2/ for solving a linear system A*x = b then use:"));
    printf(_("     x = taucs_chsolve(Cp,b)\n"));
    disp(_("  3/ for the same thing with one refinement step, use:"));
    printf(_("     xr = taucs_chsolve(Cp,b,A)\n"));
    disp(_("  4/ you may also compute the 2-norm condition number with:"));
    printf(_("     [K2, lm, vm, lM, vM] = cond2sp(A, Cp [, itermax, eps, verb])\n"));
    disp(_("  5/ if you don''t need the Cholesky factorization anymore, it is recommended to free it with taucs_chdel(Cp)"));
    printf(_("     if you have lost your pointer you may use taucs_chdel() which frees memory for all the current Cholesky factorizations.\n"));
    halt(_("\nPress Return to continue...\n"));

    mode(1)
    tic();
    Cp2 = taucs_chfact(A2);
    x2 = taucs_chsolve(Cp2,b2);
    t2_chol = toc();
    norm_res_chol_2 = norm(A2*x2-b2);
    K2_norm2 = cond2sp(A2, Cp2);
    taucs_chdel(Cp2);
    mode(-1)

    disp(_("A2 ("+descr2+"): order = "+ string(n2) + " nnz = " + string(nnz(A2))));
    printf("     K2 (1-norm) = " + string(K2) + "\n" + ..
           "     K2 (2-norm) = " + string(K2_norm2) + "\n" )
    disp(_(" with umfpack:"));
    printf(_("     norm of the residual = ") + string(norm_res2) + "\n" + ..
           _("     same but with refinment = ") + string(norm_res2r) + "\n" + ..
           _("     computing time       = ") + string(t2) + "\n")
    disp(_(" with the taucs snmf Cholesky solver:"))
    printf(_("     norm of the residual = ") + string(norm_res_chol_2) + "\n" + ..
           _("     same but with refinment = ") + string(norm_res_chol_2r) + "\n" + ..
           _("     computing time       = ") + string(t2_chol) + "\n")

    if num == max(winsid()) then
        xdel(num);
    end

endfunction

demo_sparse_matrices()
clear demo_sparse_matrices;
