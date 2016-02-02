// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [d, v] = eigs(varargin)
    lhs = argn(1);
    rhs = argn(2);
    if(rhs == 0 | rhs > 6)
        error(msprintf(gettext("%s : Wrong number of input arguments : %d to %d expected.\n"), "eigs", 1, 6));
    end

    if(rhs >= 1)
        if((typeof(varargin(1)) <> "constant")  & typeof(varargin(1)) <> "function" & (typeof(varargin(1)) <> "sparse") | varargin(1) == [])
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A full or sparse square matrix or a function expected"), "eigs", 1));
        end
    end

    if(rhs >= 1 & typeof(varargin(1)) <> "function")
        if(isreal(varargin(1)))
            resid = rand(size(varargin(1), "r"), 1);
        else
            resid = complex(rand(size(varargin(1), "r"), 1), rand(size(varargin(1), "r"), 1));
        end
        A = varargin(1);
    end

    if(rhs > 1 & typeof(varargin(1)) ==  "function")
        if(size(varargin(2)) <> 1)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A positive integer expected if the first input argument is a function."), "eigs",2));
        end
        a_real = 1;
        a_sym = 0;
        resid = rand(varargin(2),1);
        info = 0;
        Af = varargin(1);
        Asize = varargin(2);
    end

    maxiter = 300;
    tol = %eps;
    ncv = [];
    cholB = 0;
    info = 0;
    B = [];
    sigma = "LM";
    if(rhs == 1)
        if(~issparse(varargin(1)))
            info = int32(0);
        end
    else
        if(~issparse(varargin(1)) & ~issparse(varargin(2)))
            info = int32(0);
        end
    end

    if(typeof(varargin(1)) <> "function")
        select rhs
        case 1
            nev =  min(size(A, "r"), 6);
        case 2
            nev = min(size(A, "r"), 6);
            B = varargin(2);
        case 3
            B = varargin(2);
            nev = varargin(3);
        case 4
            B = varargin(2);
            nev = varargin(3);
            sigma = varargin(4);
        case 5
            B = varargin(2);
            nev = varargin(3);
            sigma = varargin(4);
            opts = varargin(5);
            if(~isstruct(opts))
                error(msprintf(gettext("%s: Wrong type for input argument #%d: A structure expected"), "eigs", 5));
            end
            if(size(intersect(fieldnames(opts), ["tol", "maxiter", "ncv", "resid", "cholB"]), "*") < size(fieldnames(opts),"*"))
                error(msprintf(gettext("%s: Wrong type for input argument: If A is a matrix, use opts with tol, maxiter, ncv, resid, cholB"), "eigs"));
            end
            if(isfield(opts, "tol"))
                tol = opts.tol;
            end
            if(isfield(opts, "maxiter"))
                maxiter = opts.maxiter;
            end
            if(isfield(opts, "ncv"))
                ncv = opts.ncv;
            end
            if(isfield(opts, "resid"))
                resid = opts.resid;
                if(issparse(varargin(1)) | issparse(varargin(2)))
                    info = 1;
                else
                    info = int32(1);
                end
                if(and(resid==0))
                    if(issparse(varargin(1)) | issparse(varargin(2)))
                        info = 0;
                    else
                        info = int32(0);
                    end
                end
            end
            if(isfield(opts,"cholB"))
                cholB = opts.cholB;
            end
        end

        select lhs
        case 1
            if(issparse(A) | issparse(B))
                d = speigs(A, B, nev, sigma, maxiter, tol, ncv, cholB, resid, info);
            else
                d = %_eigs(A, B, nev, sigma, maxiter, tol, ncv, cholB, resid, info);
            end
        case 2
            if(issparse(A) | issparse(B))
                [d, v] = speigs(A, B, nev, sigma, maxiter, tol, ncv, cholB, resid, info);
            else
                [d, v] = %_eigs(A, B, nev, sigma, maxiter, tol, ncv, cholB, resid, info);
            end
        end
    else
        select rhs
        case 2
            nev = min(Asize, 6)

        case 3
            nev = min(Asize, 6);
            B = varargin(3);

        case 4
            B = varargin(3);
            nev = varargin(4);

        case 5
            B = varargin(3);
            nev = varargin(4);
            sigma = varargin(5);

        case 6
            B = varargin(3);
            nev = varargin(4);
            sigma = varargin(5);
            opts = varargin(6);
            if(~isstruct(opts)) then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: A structure expected"), "eigs",5));
            end
            if(size(intersect(fieldnames(opts), ["tol", "maxiter", "ncv", "resid", "cholB", "issym", "isreal"]), "*") < size(fieldnames(opts),"*"))
                error(msprintf(gettext("%s: Wrong type for input argument: If A is a function, use opts with tol, maxiter, ncv, resid, cholB, issym", "isreal"), "eigs"));
            end
            if(isfield(opts,"tol"))
                tol = opts.tol;
            end
            if(isfield(opts,"maxiter"))
                maxiter = opts.maxiter;
            end
            if(isfield(opts, "ncv"))
                ncv = opts.ncv;
            end
            if(isfield(opts,"resid"))
                resid = opts.resid;
                info = 1;
                if(and(resid==0))
                    info = 0;
                end
            end
            if(isfield(opts,"cholB"))
                cholB = opts.cholB;
            end
            if(isfield(opts,"issym"))
                a_sym = opts.issym;
            end
            if(isfield(opts,"isreal"))
                a_real = opts.isreal;
                if(~a_real & ~isfield(opts,"resid"))
                    resid = complex(rand(Asize, 1), rand(Asize, 1));
                end
            end
        end
        select lhs
        case 1
            d = feigs(Af, Asize, B, nev, sigma, maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
        case 2
            [d, v] = feigs(Af, Asize, B, nev, sigma, maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
        end
    end
endfunction

function [res_d, res_v] = speigs(A, %_B, nev, which, maxiter, tol, ncv, cholB, resid, info)
    lhs = argn(1);
    rvec = 0;
    if(lhs > 1)
        rvec = 1;
    end

    //**************************
    //First variable A :
    //**************************
    [mA, nA] = size(A);

    //check if A is a square matrix
    if(mA * nA < 2 | mA <> nA)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A square matrix expected.\n"), "eigs", 1));
    end

    //check if A is complex
    Areal = isreal(A);

    //check if A is symetric
    Asym = norm(A-A') == 0;

    //*************************
    //Second variable B :
    //*************************
    if((typeof(%_B) <> "constant") & (typeof(%_B) <> "sparse"))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An empty matrix or full or sparse square matrix expected.\n"), "eigs", 2));
    end
    [mB, nB] = size(%_B);

    //Check if B is a square matrix
    if(mB * nB <> 0 & (mB <> mA | nB <> nA))
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: B must have the same size as A.\n"), "eigs", 2));
    end

    //check if B is complex
    Breal = isreal(%_B);
    matB = mB * nB;

    //*************************
    //NEV :
    //*************************
    //verification du type de nev
    //check if nev is complex?
    if(typeof(nev) <> "constant") | (~isreal(nev)) | (size(nev,"*") <> 1)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 3));
    end

    if(nev <> floor(nev) | (nev<=0))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: k must be a positive integer.\n"), "eigs", 3));
    end

    if(Asym & Areal & Breal)
        if(nev >= nA)
            error(msprintf(gettext("%s: Wrong value for input argument #%d: For real symmetric problems, k must be an integer in the range 1 to N - 1.\n"), "eigs", 3));
        end
    else
        if(nev >= nA - 1)
            error(msprintf(gettext("%s: Wrong value for input argument #%d: For real non symmetric or complex problems, k must be an integer in the range 1 to N - 2.\n"), "eigs", 3));
        end
    end

    //*************************
    //SIGMA AND WHICH :
    //*************************
    //Check type
    select type(which)
    case 1 then
        if(typeof(which) <> "constant" | which == [] | isnan(which))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 4));
        end
        sigma = which;
        which = "LM";

    case 10 then
        [mWHICH, nWHICH] = size(which);
        if(mWHICH * nWHICH <> 1)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "eigs", 4));
        end
        if(strcmp(which,"LM") ~= 0 & strcmp(which,"SM") ~= 0  & strcmp(which,"LR") ~= 0 & strcmp(which,"SR") ~= 0 & strcmp(which,"LI") ~= 0 & strcmp(which,"SI") ~= 0 & strcmp(which,"LA") ~= 0 & strcmp(which,"SA") ~= 0 & strcmp(which,"BE") ~= 0)
            if(Areal & Breal & Asym)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of ''%s'', ''%s'', ''%s'', ''%s'' or ''%s''.\n"), "eigs", 4, "LM", "SM", "LA", "SA", "BE"));
            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s''.\n"), "eigs", 4, "LM", "SM", "LR", "SR", "LI", "SI"));
            end
        end
        if((~Areal | ~Breal | ~Asym) & (strcmp(which,"LA") == 0 | strcmp(which,"SA") == 0 | strcmp(which,"BE") == 0))
            error(msprintf(gettext("%s: Invalid sigma value for complex or non symmetric problem.\n"), "eigs"));
        end
        if(Areal & Breal & Asym & (strcmp(which,"LR") == 0 | strcmp(which,"SR") == 0 | strcmp(which,"LI") == 0 | strcmp(which,"SI") == 0))
            error(msprintf(gettext("%s: Invalid sigma value for real symmetric problem.\n"), "eigs"));
        end
        sigma = 0;

    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: a real scalar or a string expected.\n"), "eigs", 4));
    end

    if(~Areal | ~Breal)
        sigma = complex(sigma);
    end

    //*************************
    //MAXITER :
    //*************************
    if(typeof(maxiter) <> "constant" | ~isreal(maxiter) | size(maxiter, "*") <> 1)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be a scalar.\n"), "eigs", 5, "opts.maxiter"));
    end

    if((maxiter <> floor(maxiter)) | (maxiter <= 0) )
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer positive value.\n"), "eigs", 5, "opts.maxiter"));
    end

    //*************************
    //TOL :
    //*************************
    if(typeof(tol) <> "constant" | ~isreal(tol) | isnan(tol))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be a real scalar.\n"), "eigs", 6, "opts.tol"));
    end

    if(size(tol, "*") <> 1)
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: %s must be 1 by 1 size.\n"), "eigs", 6, "opts.tol"));
    end

    //*************************
    //NCV :
    //*************************
    if(typeof(ncv) <> "constant" | ~isreal(ncv))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv"));
    end

    if(size(ncv, "*") > 1 | ncv <> floor(ncv) | (ncv <> [] & ncv <= 0))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv"));
    end

    if(isempty(ncv))
        if(~Asym & Areal & Breal)
            ncv = min(max(2*nev+1, 20), nA);
        else
            ncv = min(max(2*nev, 20), nA);
        end
    else
        if(ncv <= nev | ncv > nA)
            if(Asym & Areal & Breal)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: For real symmetric problems, NCV must be k < NCV <= N.\n"), "eigs", 7));
            elseif(~Asym & Areal & Breal)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: For real non symmetric problems, NCV must be k + 2 < NCV < N.\n"), "eigs", 7));
            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: For complex problems, NCV must be k + 1 < NCV <= N.\n"), "eigs", 7));
            end
        end
    end

    //*************************
    //CHOL :
    //*************************
    select typeof(cholB)
    case "boolean"
        if and(opts.cholB <> [%f %t]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be %s or %s.\n"), "eigs", 8, "opts.cholB","%f", "%t"));
        end
    case "constant"
        //check if chol is complex?
        if(~isreal(cholB) | size(cholB, "*") <> 1)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB"));
        end

        if(and(cholB <> [0 1]))
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be %s or %s.\n"), "eigs", 8, "opts.cholB","%f", "%t"));
        end
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB"));
    end

    //*************************
    //RESID :
    //*************************
    if(typeof(resid) <> "constant")
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real or complex matrix expected.\n"), "eigs", 9));
    end

    if(size(resid, "*") ~= nA)
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: Start vector %s must be N by 1.\n"), "eigs", 9, "opts.resid"));
    end

    if(Areal & Breal)
        //resid complexe ?
        if(~isreal(resid))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Start vector %s must be real for real problems.\n"), "eigs", 9, "opts.resid"));
        end
    else
        if(isreal(resid))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Start vector %s must be complex for complex problems.\n"), "eigs", 9, "opts.resid"));
        end
    end

    iparam = zeros(11,1);
    iparam(1) = 1;
    iparam(3) = maxiter;
    iparam(7) = 1;

    ipntr = zeros(14,1);

    //MODE 1, 2, 3, 4, 5
    if(~strcmp(which,"SM") | sigma <> 0)
        iparam(7) = 3;
        which = "LM";
    end

    //bmat initialization
    if(matB == 0 | iparam(7) == 1)
        bmat = "I";
    else
        bmat = "G";
    end

    if(cholB)
        if(or(triu(%_B) <> %_B))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: if opts.cholB is true, B must be upper triangular.\n"), "eigs", 2));
        end
        if(issparse(%_B)) //sparse cholesky decomposition is reversed...
            Rprime = %_B;
            R = Rprime';
        else
            R = %_B;
            Rprime = R';
        end
    end

    if(~cholB & matB & iparam(7) == 1)
        if(issparse(%_B) & ~Breal)
            error(msprintf(gettext("%s: Impossible to use the Cholesky factorization with complex sparse matrices.\n"), "eigs"));
        else
            if(issparse(%_B))
                [R, P] = spchol(%_B);
                perm = spget(P);
                perm = perm(:,2);
                iperm = spget(P');
                iperm = iperm(:,2);
            else
                R = chol(%_B);
                Rprime = R';
            end
        end
    end
    if(matB & issparse(%_B) & iparam(7) ==1)
        Rfact = umf_lufact(R);
        Rprimefact = umf_lufact(R');
    end

    //Main
    howmny = "A";
    ido = 0;
    info_eupd = 0;
    _select = zeros(ncv,1);
    if(iparam(7) == 3)
        if(matB == 0)
            AMSB = A - sigma * speye(nA, nA);
        else
            if(cholB)
                AMSB = A - (sigma * Rprime * R);
            else
                AMSB = A - sigma * %_B;
            end
        end
        Lup = umf_lufact(AMSB);
    end

    if(Areal)
        if(Asym)
            lworkl = ncv * ncv + 8 * ncv;
            v = zeros(nA, ncv);
            workl = zeros(lworkl, 1);
            workd = zeros(3 * nA, 1);
            d = zeros(nev, 1);
            z = zeros(nA, nev);
        else
            lworkl = 3 * ncv * (ncv + 2);
            v = zeros(nA, ncv);
            workl = zeros(lworkl, 1);
            workd = zeros(3 * nA, 1);
            dr = zeros(nev+1, 1);
            di = zeros(nev+1, 1);
            z = zeros(nA, nev + 1);
            workev = zeros(3 * ncv, 1);
        end
    else
        lworkl = 3 * ncv * ncv + 5 * ncv;
        v = zeros(nA, ncv) + 0 * %i;
        workl = zeros(lworkl, 1) + 0 * %i;
        workd = zeros(3 * nA, 1) + 0 * %i;
        rwork = zeros(ncv, 1);
        d = zeros(nev + 1, 1) + 0 * %i;
        z = zeros(nA, nev) + 0 * %i;
        workev = zeros(2 * ncv, 1) + 0 * %i;
    end

    while(ido <> 99)
        if(Areal & Breal)
            if(Asym)
                [ido, resid, v, iparam, ipntr, workd, workl, info] = dsaupd(ido, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info);
                if(info < 0)
                    error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DSAUPD", info));
                end
            else
                [ido, resid, v, iparam, ipntr, workd, workl, info] = dnaupd(ido, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info);
                if(info < 0)
                    error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DNAUPD", info));
                end
            end
        else
            [ido, resid, v, iparam, ipntr, workd, workl, rwork, info] = znaupd(ido, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, rwork, info);
            if(info < 0)
                error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "ZNAUPD", info));
            end
        end

        if(ido == -1 | ido == 1 | ido == 2)
            if(iparam(7) == 1)
                if(ido == 2)
                    workd(ipntr(2):ipntr(2)+nA-1) = workd(ipntr(1):ipntr(1)+nA-1);
                else
                    if(matB == 0)
                        workd(ipntr(2):ipntr(2)+nA-1) = A * workd(ipntr(1):ipntr(1)+nA-1);
                    else
                        if(issparse(%_B))
                            y = umf_lusolve(Rprimefact, workd(ipntr(1):ipntr(1)+nA-1));
                            if(~cholB)
                                y = A * y(perm);
                                y = y(iperm);
                            else
                                y = A * y;
                            end
                            workd(ipntr(2):ipntr(2)+nA-1) = umf_lusolve(Rfact, y);
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime \ (A * (R \ workd(ipntr(1):ipntr(1)+nA-1)));
                        end
                    end
                end
            elseif(iparam(7) == 3)
                if(matB == 0)
                    if(ido == 2)
                        workd(ipntr(2):ipntr(2)+nA-1) = workd(ipntr(1):ipntr(1)+nA-1);
                    else
                        workd(ipntr(2):ipntr(2)+nA-1) = umf_lusolve(Lup, workd(ipntr(1):ipntr(1)+nA-1));
                    end
                else
                    if(ido == 2)
                        if(cholB)
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime * (R * workd(ipntr(1):ipntr(1)+nA-1));
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = %_B * workd(ipntr(1):ipntr(1)+nA-1);
                        end
                    elseif(ido == -1)
                        if(cholB)
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime * (R * workd(ipntr(1):ipntr(1)+nA-1));
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = %_B * workd(ipntr(1):ipntr(1)+nA-1);
                        end
                        workd(ipntr(2):ipntr(2)+nA-1) = umf_lusolve(Lup, workd(ipntr(2):ipntr(2)+nA-1));
                    else
                        workd(ipntr(2):ipntr(2)+nA-1) = umf_lusolve(Lup, workd(ipntr(3):ipntr(3)+nA-1));
                    end
                end
            else
                if(Areal & Breal)
                    if(Asym)
                        error(msprintf(gettext("%s: Error with %s: unknown mode returned.\n"), "eigs", "DSAUPD"));
                    else
                        error(msprintf(gettext("%s: Error with %s: unknown mode returned.\n"), "eigs", "DNAUPD"));
                    end
                else
                    error(msprintf(gettext("%s: Error with %s: unknown mode returned.\n"), "eigs", "ZNAUPD"));
                end
            end
        end
    end
    if(iparam(7) == 3)
        umf_ludel(Lup);
    end
    if(Areal & Breal)
        if(Asym)
            [d, z, resid, v, iparam, iptnr, workd, workl, info_eupd] = dseupd(rvec, howmny, _select, d, z, sigma, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info_eupd);
            if(info_eupd <> 0)
                error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DSEUPD", info_eupd));
            else
                res_d = d;
                if(rvec)
                    res_d = diag(res_d);
                    res_v = z;
                end
            end
        else
            sigmar = real(sigma);
            sigmai = imag(sigma);
            computevec = rvec;
            if iparam(7) == 3 & sigmai then
                computevec = 1;
            end
            [dr, di, z, resid, v, iparam, ipntr, workd, workl, info_eupd] = dneupd(computevec, howmny, _select, dr, di, z, sigmar, sigmai, workev, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info_eupd);
            if(info_eupd <> 0)
                error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DNEUPD", info_eupd));
            else
                if iparam(7) == 3 & sigmai then
                    res_d = complex(zeros(nev + 1,1));
                    i = 1;
                    while i <= nev
                        if(~di(i))
                            res_d(i) = complex(z(:,i)'*A*z(:,i), 0);
                            i = i + 1;
                        else
                            real_part = z(:,i)' * A * z(:,i) + z(:,i+1)' * A * z(:,i+1);
                            imag_part = z(:,i)' * A * z(:,i+1) - z(:,i+1)' * A * z(:,i)
                            res_d(i) = complex(real_part, imag_part);
                            res_d(i+1) = complex(real_part, -imag_part);
                            i = i + 2;
                        end
                    end
                else
                    res_d = complex(dr, di);
                end
                res_d = res_d(1:nev);
                if(rvec)
                    index = find(di~=0);
                    index = index(1:2:$);
                    res_v = z;
                    if ~isempty(index) then
                        res_v(:,[index index+1]) = [complex(res_v(:,index),res_v(:,index+1)), complex(res_v(:,index),-res_v(:,index+1))];
                    end
                    res_d = diag(res_d);
                    res_v = res_v(:,1:nev);
                end
            end
        end
    else
        [d, z, resid, iparam, ipntr, workd, workl, rwork, info_eupd] = zneupd(rvec, howmny, _select, d, z, sigma, workev, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, rwork, info_eupd);
        if(info_eupd <> 0)
            error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "ZNEUPD", info_eupd));
        else
            d(nev+1) = []
            res_d = d;
            if(rvec)
                res_d = diag(d);
                res_v = z;
            end
        end
    end
    if(rvec & iparam(7) == 1 & matB)
        if issparse(%_B)
            res_v = umf_lusolve(Rprimefact, res_v);
            if(~cholB)
                res_v = res_v(perm, :);
            end
        else
            res_v = R \ res_v;
        end
    end
endfunction


function [res_d, res_v] = feigs(A_fun, nA, %_B, nev, which, maxiter, tol, ncv, cholB, resid, info, a_real, a_sym)
    lhs = argn(1);
    rvec = 0;
    if(lhs > 1)
        rvec = 1;
    end

    //**************************
    //Second variable nA :
    //**************************
    if(size(nA,"*") <> 1 | ~isreal(nA) | typeof(nA) <> "constant")
        error(msprintf(gettext("%s: Wrong type for input argument #%d: n must be a positive integer.\n"), "eigs", 2));
    end

    if(floor(nA) <> nA | nA <= 0)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: n must be a positive integer.\n"), "eigs", 2));
    end

    //*************************
    //Third variable B :
    //*************************
    if((typeof(%_B) <> "constant") & (typeof(%_B) <> "sparse"))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An empty matrix or full or sparse square matrix expected.\n"), "eigs", 3));
    end
    [mB, nB] = size(%_B);

    matB = mB * nB;
    //Check if B is a square matrix
    if(matB & (mB <> nA |nB <> nA))
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: B must have the same size as A.\n"), "eigs", 3));
    end

    //check if B is complex
    Breal = isreal(%_B);

    //*************************
    //NEV :
    //*************************
    // type of nev check
    // check if nev is complex?
    if(typeof(nev) <> "constant") | (~isreal(nev)) | (size(nev,"*") <> 1)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 3));
    end

    if(nev <> floor(nev) | (nev<=0))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: k must be a positive integer.\n"), "eigs", 4));
    end

    if(a_sym & a_real & Breal)
        if(nev >= nA)
            error(msprintf(gettext("%s: Wrong value for input argument #%d: For real symmetric problems, k must be in the range 1 to N - 1.\n"), "eigs", 4));
        end
    else
        if(nev >= nA - 1)
            error(msprintf(gettext("%s: Wrong value for input argument #%d: For real non symmetric or complex problems, k must be in the range 1 to N - 2.\n"), "eigs", 4));
        end
    end

    //*************************
    //SIGMA AND WHICH :
    //*************************
    //Check type
    select type(which)
    case 1 then
        if(typeof(which) <> "constant" | which == [] | isnan(which))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a scalar expected.\n"), "eigs", 5));
        end
        sigma = which;
        which = "LM";
    case 10 then
        [mWHICH, nWHICH] = size(which);
        if(mWHICH * nWHICH <> 1)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), "eigs", 5));
        end
        if(strcmp(which,"LM") ~= 0 & strcmp(which,"SM") ~= 0  & strcmp(which,"LR") ~= 0 & strcmp(which,"SR") ~= 0 & strcmp(which,"LI") ~= 0 & strcmp(which,"SI") ~= 0 & strcmp(which,"LA") ~= 0 & strcmp(which,"SA") ~= 0 & strcmp(which,"BE") ~= 0)
            if(a_real & Breal & a_sym)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of %s, %s, %s, %s or %s.\n"), "eigs", 5, "LM", "SM", "LA", "SA", "BE"));
            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of %s, %s, %s, %s, %s or %s.\n"), "eigs", 5, "LM", "SM", "LR", "SR", "LI", "SI"));
            end
        end
        if((~a_real | ~Breal | ~a_sym) & (strcmp(which,"LA") == 0 | strcmp(which,"SA") == 0 | strcmp(which,"BE") == 0))
            error(msprintf(gettext("%s: Invalid sigma value for complex or non symmetric problem.\n"), "eigs"));
        end
        if(a_real & Breal & a_sym & (strcmp(which,"LR") == 0 | strcmp(which,"SR") == 0 | strcmp(which,"LI") == 0 | strcmp(which,"SI") == 0))
            error(msprintf(gettext("%s: Invalid sigma value for real symmetric problem.\n"), "eigs"));
        end
        sigma = 0;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: a real scalar or a string expected.\n"), "eigs", 5));
    end

    if(~a_real | ~Breal)
        sigma = complex(sigma);
    end

    //*************************
    //MAXITER :
    //*************************
    if(typeof(maxiter) <> "constant" | ~isreal(maxiter) | size(maxiter,"*") <> 1)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be a scalar.\n"), "eigs", 6, "opts.maxiter"));
    end

    if((maxiter <> floor(maxiter)) | (maxiter <= 0) )
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer positive value.\n"), "eigs", 6, "opts.maxiter"));
    end

    //*************************
    //TOL :
    //*************************
    if(typeof(tol) <> "constant" | ~isreal(tol) | isnan(tol))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be a real scalar.\n"), "eigs", 7, "opts.tol"));
    end

    if(size(tol,"*") <> 1)
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: %s must be 1 by 1 size.\n"), "eigs", 7, "opts.tol"));
    end

    //*************************
    //NCV :
    //*************************
    if(typeof(ncv) <> "constant" | ~isreal(ncv))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 8, "opts.ncv"));
    end

    if(size(ncv,"*") > 1 | ncv <> floor(ncv) | (ncv <> [] & ncv <= 0))
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: %s must be an integer scalar.\n"), "eigs", 8, "opts.ncv"));
    end

    if(isempty(ncv))
        if(~a_sym & a_real & Breal)
            ncv = min(max(2*nev+1, 20), nA);
        else
            ncv = min(max(2*nev, 20), nA);
        end
    else
        if(ncv <= nev | ncv > nA)
            if(a_sym & a_real & Breal)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: For real symmetric problems, NCV must be k < NCV <= N.\n"), "eigs", 8));
            elseif(~a_sym & a_real & Breal)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: For real non symmetric problems, NCV must be k + 2 < NCV < N.\n"), "eigs", 8));

            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: For complex problems, NCV must be k + 1 < NCV <= N.\n"), "eigs", 8));
            end
        end
    end

    //*************************
    //CHOL :
    //*************************
    select typeof(cholB)
    case "boolean"
        if and(cholB <> [%f %t]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be %s or %s.\n"), "eigs", 8, "opts.cholB","%f", "%t"));
        end
    case "constant"
        //check if chol is complex?
        if(~isreal(cholB) | size(cholB, "*") <> 1)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB"));

        end

        if(and(cholB <> [0 1]))
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %s must be %s or %s.\n"), "eigs", 8, "opts.cholB","%f", "%t"));
        end
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB"));
    end

    //*************************
    //RESID :
    //*************************
    if(typeof(resid) <> "constant")
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real or complex matrix expected.\n"), "eigs", 10));
    end

    if(size(resid,"*") ~= nA)
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: Start vector opts.resid must be N by 1.\n"), "eigs", 10));
    end

    if(a_real & Breal)
        //resid complex ?
        if(~isreal(resid))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Start vector opts.resid must be real for real problems.\n"), "eigs", 10));
        end
    else
        if(isreal(resid))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Start vector opts.resid must be complex for complex problems.\n"), "eigs", 10));
        end
    end

    iparam = zeros(11,1);
    iparam(1) = 1;
    iparam(3) = maxiter;
    iparam(7) = 1;

    ipntr = zeros(14,1);

    //MODE 1, 2, 3, 4, 5
    if(~strcmp(which,"SM") | sigma <> 0)
        iparam(7) = 3;
        which = "LM";
    end

    //bmat initialization
    if(matB == 0 | iparam(7) == 1)
        bmat = "I";
    else
        bmat = "G";
    end

    if(cholB)
        if(or(triu(%_B) <> %_B))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: if opts.cholB is true, B must be upper triangular.\n"), "eigs", 2));
        end
        if(issparse(%_B)) //sparse cholesky decomposition is reversed...
            Rprime = %_B;
            R = Rprime;
        else
            R = %_B;
            Rprime = R';
        end
    end
    if(~cholB & matB & iparam(7) == 1)
        if(issparse(%_B) & ~Breal)
            error(msprintf(gettext("%s: Impossible to use the Cholesky factorization with complex sparse matrices.\n"), "eigs"));
        else
            if(issparse(%_B))
                [R,P] = spchol(%_B);
                perm = spget(P);
                perm = perm(:,2);
                iperm = spget(P');
                iperm = iperm(:,2);
            else
                R = chol(%_B);
                Rprime = R';
            end
        end
    end
    if(matB & issparse(%_B) & iparam(7) == 1)
        Rfact = umf_lufact(R);
        Rprimefact = umf_lufact(R');
    end

    //Main
    howmny = "A";
    ido = 0;
    info_aupd = 0;
    _select = zeros(ncv,1);

    if(a_real)
        if(a_sym)
            lworkl = ncv * ncv + 8 * ncv;
            v = zeros(nA, ncv);
            workl = zeros(lworkl, 1);
            workd = zeros(3 * nA, 1);
            d = zeros(nev, 1);
            z = zeros(nA, nev);
        else
            lworkl = 3 * ncv * (ncv + 2);
            v = zeros(nA, ncv);
            workl = zeros(lworkl, 1);
            workd = zeros(3 * nA, 1);
            dr = zeros(nev+1, 1);
            di = zeros(nev+1, 1);
            z = zeros(nA, nev + 1);
            workev = zeros(3 * ncv, 1);
        end
    else
        lworkl = 3 * ncv * ncv + 5 * ncv;
        v = zeros(nA, ncv) + 0 * %i;
        workl = zeros(lworkl, 1) + 0 * %i;
        workd = zeros(3 * nA, 1) + 0 * %i;
        rwork = zeros(ncv, 1);
        d = zeros(nev + 1, 1) + 0 * %i;
        z = zeros(nA, nev) + 0 * %i;
        workev = zeros(2 * ncv, 1) + 0 * %i;
    end
    while(ido <> 99)
        if(a_real & Breal)
            if(a_sym)
                [ido, resid, v, iparam, ipntr, workd, workl, info] = dsaupd(ido, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info_aupd);
                if(info_aupd <0)
                    error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DSAUPD", info_aupd));
                end
            else
                [ido, resid, v, iparam, ipntr, workd, workl, info] = dnaupd(ido, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info_aupd);
                if(info_aupd <0)
                    error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DNAUPD", info_aupd));
                end
            end
        else
            [ido, resid, v, iparam, ipntr, workd, workl, rwork, info] = znaupd(ido, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, rwork, info_aupd);
            if(info_aupd <0)
                error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "ZNAUPD", info_aupd));
            end
        end

        if(ido == -1 | ido == 1 | ido == 2)
            if(iparam(7) == 1)
                if(ido == 2)
                    workd(ipntr(2):ipntr(2)+nA-1) = workd(ipntr(1):ipntr(1)+nA-1);
                else
                    if(matB == 0)
                        ierr = execstr("workd(ipntr(2):ipntr(2)+nA-1) = A_fun(workd(ipntr(1):ipntr(1)+nA-1))", "errcatch");
                        if(ierr <> 0)
                            break;
                        end
                    else
                        if(issparse(%_B))
                            y = umf_lusolve(Rprimefact, workd(ipntr(1):ipntr(1)+nA-1));
                            if(~cholB)
                                ierr = execstr("workd(ipntr(2):ipntr(2)+nA-1) = A_fun( y(perm) )", "errcatch");
                                if(ierr <> 0)
                                    break;
                                end
                                y = y(iperm);
                            else
                                ierr = execstr("workd(ipntr(2):ipntr(2)+nA-1) = A_fun(y)", "errcatch");
                                if(ierr <> 0)
                                    break;
                                end
                            end
                            workd(ipntr(2):ipntr(2)+nA-1) = umf_lusolve(Rfact, y);
                        else
                            ierr = execstr("workd(ipntr(2):ipntr(2)+nA-1) = A_fun( R \ workd(ipntr(1):ipntr(1)+nA-1) )", "errcatch");
                            if(ierr <> 0)
                                break;
                            end
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime \ workd(ipntr(2):ipntr(2)+nA-1);
                        end
                    end
                end
            elseif(iparam(7) == 3)
                if(matB == 0)
                    if(ido == 2)
                        workd(ipntr(2):ipntr(2)+nA-1) = workd(ipntr(1):ipntr(1)+nA-1);
                    else
                        ierr = execstr("workd(ipntr(2):ipntr(2)+nA-1) = A_fun(workd(ipntr(1):ipntr(1)+nA-1))", "errcatch");
                        if(ierr <> 0)
                            break;
                        end
                    end
                else
                    if(ido == 2)
                        if(cholB)
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime * (R * workd(ipntr(1):ipntr(1)+nA-1));
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = %_B * workd(ipntr(1):ipntr(1)+nA-1);
                        end
                    elseif(ido == -1)
                        if(cholB)
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime * (R * workd(ipntr(1):ipntr(1)+nA-1));
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = %_B * workd(ipntr(1):ipntr(1)+nA-1);
                        end
                        ierr = execstr("workd(ipntr(2):ipntr(2)+nA-1) = A_fun(workd(ipntr(2):ipntr(2)+nA-1))", "errcatch");
                        if(ierr <> 0)
                            break;
                        end
                    else
                        ierr = execstr("workd(ipntr(2):ipntr(2)+nA-1) = A_fun(workd(ipntr(3):ipntr(3)+nA-1))", "errcatch");
                        if(ierr <> 0)
                            break;
                        end
                    end
                end
            else
                if(a_real & Breal)
                    if(a_sym)
                        error(msprintf(gettext("%s: Error with %s: unknown mode returned.\n"), "eigs", "DSAUPD"));
                    else
                        error(msprintf(gettext("%s: Error with %s: unknown mode returned.\n"), "eigs", "DNAUPD"));
                    end
                else
                    error(msprintf(gettext("%s: Error with %s: unknown mode returned.\n"), "eigs", "ZNAUPD"));
                end
            end
        end
    end

    if(ierr <> 0)
        if(ierr == 10)
            error(msprintf(gettext("%s: Wrong value for input argument #%d: n does not match rows number of matrix A.\n"), "eigs", 2));
        end
        error(msprintf(gettext("%s: Wrong type or value for input arguments.\n"), "eigs"));
    end

    if(a_real & Breal)
        if(a_sym)
            [d, z, resid, v, iparam, iptnr, workd, workl, info_eupd] = dseupd(rvec, howmny, _select, d, z, sigma, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info);
            if(info <> 0)
                error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DSEUPD", info));
            else
                res_d = d;
                if(rvec)
                    res_d = diag(res_d);
                    res_v = z;
                end
            end
        else
            sigmar = real(sigma);
            sigmai = imag(sigma);
            computevec = rvec;
            if iparam(7) == 3 & sigmai then
                computevec = 1;
            end
            [dr, di, z, resid, v, iparam, ipntr, workd, workl, info_eupd] = dneupd(computevec, howmny, _select, dr, di, z, sigmar, sigmai, workev, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info);
            if(info <> 0)
                error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DNEUPD", info));
            else
                if iparam(7) == 3 & sigmai then
                    res_d = complex(zeros(nev + 1,1));
                    i = 1;
                    while i <= nev
                        if(~di(i))
                            res_d(i) = complex(z(:,i)'*A*z(:,i), 0);
                            i = i + 1;
                        else
                            real_part = z(:,i)' * A * z(:,i) + z(:,i+1)' * A * z(:,i+1);
                            imag_part = z(:,i)' * A * z(:,i+1) - z(:,i+1)' * A * z(:,i)
                            res_d(i) = complex(real_part, imag_part);
                            res_d(i+1) = complex(real_part, -imag_part);
                            i = i + 2;
                        end
                    end
                else
                    res_d = complex(dr,di);
                end
                res_d = res_d(1:nev);
                if(rvec)
                    index = find(di~=0);
                    index = index(1:2:$);
                    res_v = z;
                    if ~isempty(index) then
                        res_v(:,[index index+1]) = [complex(res_v(:,index), res_v(:,index+1)), complex(res_v(:,index), -res_v(:,index+1))];
                    end
                    res_d = diag(res_d);
                    res_v = res_v(:,1:nev);
                end
            end
        end
    else
        [d, z, resid, iparam, ipntr, workd, workl, rwork, info_eupd] = zneupd(rvec, howmny, _select, d, z, sigma, workev, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, rwork, info);
        if(info <> 0)
            error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "ZNEUPD", info));
        else
            d(nev+1) = []
            res_d = d;
            if(rvec)
                res_d = diag(d);
                res_v = z;
            end
        end
    end
    if(rvec & iparam(7) == 1 & matB)
        if issparse(%_B)
            res_v = umf_lusolve(Rprimefact, res_v);
            if(~cholB)
                res_v = res_v(perm, :);
            end
        else
            res_v = R \ res_v;
        end
    end
endfunction
