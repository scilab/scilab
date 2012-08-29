// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
            resid = rand(size(varargin(1), "r"), 1).* %i;
        end
    end

    if(rhs > 1 & typeof(varargin(1)) ==  "function")
        if(size(varargin(2)) <> 1)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A positive integer expected if the first input argument is a function."), "eigs",2));
        end
        a_real = 1;
        a_sym = 0;
        resid = rand(varargin(2),1);
        info = 0;
    end

    maxiter = 300;
    tol = %eps;
    ncv = [];
    cholB = 0;
    info = 0;

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
            nev =  min(size(varargin(1), 'r'), 6)
            select lhs
            case 1
                if(issparse(varargin(1)))
                    d = speigs(varargin(1), [], nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
                else
                    d = %_eigs(varargin(1), [], nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
                end
            case 2
                if(issparse(varargin(1)))
                    [d, v] = speigs(varargin(1), [], nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
                else
                    [d, v] = %_eigs(varargin(1), [], nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
                end
            end

        case 2
            nev = min(size(varargin(1), 'r'), 6)
            select lhs
            case 1
                if(issparse(varargin(1)) | issparse(varargin(2)))
                    d = speigs(varargin(1), varargin(2), nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
                else
                    d = %_eigs(varargin(1), varargin(2), nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
                end
            case 2
                if(issparse(varargin(1)) | issparse(varargin(2)))
                    [d, v] = speigs(varargin(1), varargin(2), nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
                else
                    [d, v] = %_eigs(varargin(1), varargin(2), nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
                end
            end

        case 3
            select lhs
            case 1
                if(issparse(varargin(1)) | issparse(varargin(2)))
                    d = speigs(varargin(1), varargin(2), varargin(3), 'LM', maxiter, tol, ncv, cholB, resid, info);
                else
                    d = %_eigs(varargin(1), varargin(2), varargin(3), 'LM', maxiter, tol, ncv, cholB, resid, info);
                end
            case 2
                if(issparse(varargin(1)) | issparse(varargin(2)))
                    [d, v] = speigs(varargin(1), varargin(2), varargin(3), 'LM', maxiter, tol, ncv, cholB, resid, info);
                else
                    [d, v] = %_eigs(varargin(1), varargin(2), varargin(3), 'LM', maxiter, tol, ncv, cholB, resid, info);
                end
            end

        case 4
            select lhs
            case 1
                if(issparse(varargin(1)) | issparse(varargin(2)))
                    d = speigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
                else
                    d = %_eigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
                end
            case 2
                if(issparse(varargin(1)) | issparse(varargin(2)))
                    [d, v] = speigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
                else
                    [d, v] = %_eigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
                end
            end

        case 5
            select lhs
            case 1
                opts = varargin(5);
                if(~isstruct(opts))
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: A structure expected"), "eigs", 5));
                end
                if(and(~isfield(opts, ["tol", "maxiter", "ncv", "resid", "cholB"])))
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
                if(issparse(varargin(1)) | issparse(varargin(2)))
                    d = speigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
                else
                    d = %_eigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
                end
            case 2
                opts = varargin(5);
                if(~isstruct(opts))
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: A structure expected"), "eigs",5));
                end
                if(and(~isfield(opts, ["tol", "maxiter", "ncv", "resid", "cholB"])))
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
                if(isfield(opts, "cholB"))
                    cholB = opts.cholB;
                end
                if(issparse(varargin(1)))
                    [d, v] = speigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
                else
                    [d, v] = %_eigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
                end
            end
        end
    else
        select rhs
        case 2
            nev = min(varargin(2), 6)
            select lhs
            case 1
                d = feigs(varargin(1), varargin(2), [], nev, 'LM', maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            case 2
                [d, v] = feigs(varargin(1), varargin(2), [], nev, 'LM', maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            end
        case 3
            nev = min(varargin(2), 6);
            select lhs
            case 1
                d = feigs(varargin(1), varargin(2), varargin(3), nev, 'LM', maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            case 2
                [d, v] = feigs(varargin(1), varargin(2), varargin(3), nev, 'LM', maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            end

        case 4
            select lhs
            case 1
                d = feigs(varargin(1), varargin(2), varargin(3), varargin(4), 'LM', maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            case 2
                [d, v] = feigs(varargin(1), varargin(2), varargin(3), varargin(4), 'LM', maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            end

        case 5
            select lhs
            case 1
                d = feigs(varargin(1), varargin(2), varargin(3), varargin(4), varargin(5), maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            case 2
                [d, v] = feigs(varargin(1), varargin(2), varargin(3), varargin(4), varargin(5), maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            end

        case 6
            select lhs
            case 1
                opts = varargin(6);
                if(~isstruct(opts)) then
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: A structure expected"), "eigs",5));
                end
                if(and(~isfield(opts, ["tol", "maxiter", "ncv", "resid", "cholB", "issym", "isreal"])))
                    error(msprintf(gettext("%s: Wrong type for input argument: Use opts with tol, maxiter, ncv, resid, cholB, issym, isreal"), "eigs"));
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
                        resid = rand(varargin(2),1).*%i;
                    end
                end

                d = feigs(varargin(1), varargin(2), varargin(3), varargin(4), varargin(5), maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            case 2
                opts = varargin(6);
                if (~isstruct(opts)) then
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: A structure expected"), "eigs",5));
                end
                if (and(~isfield(opts, ["tol", "maxiter", "ncv", "resid", "cholB" ])))
                    error(msprintf(gettext("%s: Wrong type for input argument: Use opts with tol, maxiter, ncv, resid, cholB, issym, isreal"), "eigs"));
                end
                if (isfield(opts,"tol"))
                    tol = opts.tol;
                end
                if (isfield(opts,"maxiter"))
                    maxiter = opts.maxiter;
                end
                if (isfield(opts, "ncv"))
                    ncv = opts.ncv;
                end
                if(isfield(opts,"resid"))
                    resid = opts.resid;
                    info = 1;
                    if(and(resid==0))
                        info = 0;
                    end
                end
                if (isfield(opts,"cholB"))
                    cholB = opts.cholB;
                end
                if (isfield(opts,"isreal"))
                    a_real = opts.isreal;
                    if(~a_real & ~isfield(opts,"resid"))
                        resid = rand(varargin(2),1).*%i;
                    end
                end
                if (isfield(opts,"issym"))
                    a_sym = opts.issym;
                end
                [d, v] = feigs(varargin(1), varargin(2), varargin(3), varargin(4), varargin(5), maxiter, tol, ncv, cholB, resid, info, a_real, a_sym);
            end
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
    Asym = and(A == A');

    //*************************
    //Second variable B :
    //*************************
    if((typeof(%_B) <> "constant") & (typeof(%_B) <> "sparse"))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An empty matrix or full or sparse square matrix expected.\n"), "eigs", 2));
    end
    [mB, nB] = size(%_B);

    //Check if B is a square matrix
    if(mB * nB == 1 | mB <> nB)
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
        which = 'LM';

    case 10 then
        [mWHICH, nWHICH] = size(which);
        if(mWHICH * nWHICH <> 1)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "eigs", 4));
        end
        if(strcmp(which,'LM') ~= 0 & strcmp(which,'SM') ~= 0  & strcmp(which,'LR') ~= 0 & strcmp(which,'SR') ~= 0 & strcmp(which,'LI') ~= 0 & strcmp(which,'SI') ~= 0 & strcmp(which,'LA') ~= 0 & strcmp(which,'SA') ~= 0 & strcmp(which,'BE') ~= 0)
            if(Areal & Breal & Asym)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of ''%s'', ''%s'', ''%s'', ''%s'' or ''%s''.\n"), "eigs", 4, "LM", "SM", "LA", "SA", "BE"));
            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s''.\n"), "eigs", 4, "LM", "SM", "LR", "SR", "LI", "SI"));
            end
        end
        if((~Areal | ~Breal | ~Asym) & (strcmp(which,'LA') == 0 | strcmp(which,'SA') == 0 | strcmp(which,'BE') == 0))
            error(msprintf(gettext("%s: Invalid sigma value for complex or non symmetric problem.\n"), "eigs"));
        end
        if(Areal & Breal & Asym & (strcmp(which,'LR') == 0 | strcmp(which,'SR') == 0 | strcmp(which,'LI') == 0 | strcmp(which,'SI') == 0))
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
    if(~strcmp(which,'SM') | sigma <> 0)
        iparam(7) = 3;
        which = 'LM';
    end

    //bmat initialization
    if(matB == 0 | iparam(7) == 1)
        bmat = 'I';
    else
        bmat = 'G';
    end

    if(cholB)
        if(~and(triu(%_B) == %_B))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: B must be symmetric or hermitian, definite, semi positive.\n"), "eigs", 2));
        end
        R = %_B;
        Rprime = R';
    end

    if(~cholB & matB <> 0 & iparam(7) == 1)
        if(~Breal)
            error(msprintf(gettext("%s: Impossible to use the Cholesky factorisation with complex sparse matrices.\n"), "eigs"));
        else
            [R,P] = spchol(%_B);
        end
        Rprime = R';
    end

    //Main
    howmny = 'A';
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
        if(~isreal(AMSB))
            Lup = umf_lufact(AMSB);
            [L, U, p, q, R] = umf_luget(Lup);
            R = diag(R);
            P = zeros(nA, nA);
            Q = zeros(nA, nA);
            for i = 1:nA
                P(i,p(i)) = 1;
                Q(q(i),i) = 1;
            end
            umf_ludel(Lup);
        else
            [hand, rk] = lufact(AMSB);
            [P, L, U, Q] = luget(hand);
            ludel(hand);
        end
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
                if(matB == 0)
                    workd(ipntr(2):ipntr(2)+nA-1) = A * workd(ipntr(1):ipntr(1)+nA-1);
                else
                    workd(ipntr(2):ipntr(2)+nA-1) = inv(Rprime) * A * inv(R) * workd(ipntr(1):ipntr(1)+nA-1);
                end
            elseif(iparam(7) == 3)
                if(matB == 0)
                    if(ido == 2)
                        workd(ipntr(2):ipntr(2)+nA-1) = workd(ipntr(1):ipntr(1)+nA-1);
                    else
                        if(Areal & Breal)
                            workd(ipntr(2):ipntr(2)+nA-1) = inv(Q) * inv(U) * inv(L) * inv(P) *workd(ipntr(1):ipntr(1)+nA-1);
                        else
                            if(~isreal(L) | ~isreal(U))
                                error(msprintf(gettext("%s: Impossible to invert complex sparse matrix.\n"), "eigs"));
                            else
                                workd(ipntr(2):ipntr(2)+nA-1) = Q * inv(U) * inv(L) * P * inv(R) * workd(ipntr(1):ipntr(1)+nA-1);
                            end
                        end
                    end
                else
                    if(ido == 2)
                        if(cholB)
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime * R * workd(ipntr(1):ipntr(1)+nA-1);
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = %_B * workd(ipntr(1):ipntr(1)+nA-1);
                        end
                    elseif(ido == -1)
                        if(cholB)
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime * R * workd(ipntr(1):ipntr(1)+nA-1);
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = %_B * workd(ipntr(1):ipntr(1)+nA-1);
                        end
                        if(Areal & Breal)
                            workd(ipntr(2):ipntr(2)+nA-1) = inv(Q) * inv(U) * inv(L) * inv(P) *workd(ipntr(2):ipntr(2)+nA-1);
                        else
                            if(~isreal(L) | ~isreal(U))
                                error(msprintf(gettext("%s: Impossible to invert complex sparse matrix.\n"), "eigs"));
                            else
                                workd(ipntr(2):ipntr(2)+nA-1) = Q * inv(U) * inv(L) * P * inv(R) * workd(ipntr(2):ipntr(2)+nA-1);
                            end
                        end
                    else
                        if(Areal & Breal)
                            workd(ipntr(2):ipntr(2)+nA-1) = inv(Q) * inv(U) * inv(L) * inv(P) * workd(ipntr(3):ipntr(3)+nA-1);
                        else
                            if(~isreal(L) | ~isreal(U))
                                error(msprintf(gettext("%s: Impossible to invert complex sparse matrix.\n"), "eigs"));
                            else
                                workd(ipntr(2):ipntr(2)+nA-1) = Q * inv(U) * inv(L) * P * inv(R) * workd(ipntr(3):ipntr(3)+nA-1);
                            end
                        end
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
            [dr, di, z, resid, v, iparam, ipntr, workd, workl, info_eupd] = dneupd(rvec, howmny, _select, dr, di, z, sigmar, sigmai, workev, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info_eupd);
            if(info_eupd <> 0)
                error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DNEUPD", info_eupd));
            else
                res_d = complex(dr,di);
                res_d(nev+1) = [];
                if(rvec)
                    res_d = diag(res_d)
                    res_v = z;
                    c1 = 1:2:nev + 1;
                    c2 = 2:2:nev + 1;
                    if(modulo(nev + 1, 2) == 1)
                        c1($) = [];
                    end
                    res_v(:,[c1, c2]) = [res_v(:,c1) + res_v(:,c2) * %i res_v(:,c1) - res_v(:,c2) * %i];
                    res_v(:,$) = [];
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
    if(size(nA,'*') <> 1 | ~isreal(nA) | typeof(nA) <> "constant")
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

    //Check if B is a square matrix
    if(mB * nB == 1 | mB <> nB)
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: B must have the same size as A.\n"), "eigs", 3));
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
        which = 'LM';
    case 10 then
        [mWHICH, nWHICH] = size(which);
        if(mWHICH * nWHICH <> 1)
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), "eigs", 5));
        end
        if(strcmp(which,'LM') ~= 0 & strcmp(which,'SM') ~= 0  & strcmp(which,'LR') ~= 0 & strcmp(which,'SR') ~= 0 & strcmp(which,'LI') ~= 0 & strcmp(which,'SI') ~= 0 & strcmp(which,'LA') ~= 0 & strcmp(which,'SA') ~= 0 & strcmp(which,'BE') ~= 0)
            if(a_real & Breal & a_sym)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of %s, %s, %s, %s or %s.\n"), "eigs", 5, "LM", "SM", "LA", "SA", "BE"));
            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of %s, %s, %s, %s, %s or %s.\n"), "eigs", 5, "LM", "SM", "LR", "SR", "LI", "SI"));
            end
        end
        if((~a_real | ~Breal | ~a_sym) & (strcmp(which,'LA') == 0 | strcmp(which,'SA') == 0 | strcmp(which,'BE') == 0))
            error(msprintf(gettext("%s: Invalid sigma value for complex or non symmetric problem.\n"), "eigs"));
        end
        if(a_real & Breal & a_sym & (strcmp(which,'LR') == 0 | strcmp(which,'SR') == 0 | strcmp(which,'LI') == 0 | strcmp(which,'SI') == 0))
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
    if(typeof(maxiter) <> "constant" | ~isreal(maxiter) | size(maxiter,'*') <> 1)
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

    if(size(tol,'*') <> 1)
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: %s must be 1 by 1 size.\n"), "eigs", 7, "opts.tol"));
    end

    //*************************
    //NCV :
    //*************************
    if(typeof(ncv) <> "constant" | ~isreal(ncv))
        error(msprintf(gettext("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 8, "opts.ncv"));
    end

    if(size(ncv,'*') > 1 | ncv <> floor(ncv) | (ncv <> [] & ncv <= 0))
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

    if(size(resid,'*') ~= nA)
        error(msprintf(gettext("%s: Wrong dimension for input argument #%d: Start vector opts.resid must be N by 1.\n"), "eigs", 10));
    end

    if(a_real & Breal)
        //resid complexe ?
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
    if(~strcmp(which,'SM') | sigma <> 0)
        iparam(7) = 3;
        which = 'LM';
    end

    //bmat initialization
    if(matB == 0 | iparam(7) == 1)
        bmat = 'I';
    else
        bmat = 'G';
    end

    if(cholB)
        if(~and(triu(%_B) == %_B))
            error(msprintf(gettext("%s: Wrong type for input argument #%d: B must be symmetric or hermitian, definite, semi positive.\n"), "eigs", 2));
        end
        R = %_B;
        Rprime = R';
    end

    if(~cholB & matB <> 0 & iparam(7) == 1)
        if(~Breal)
            error(msprintf(gettext("%s: Impossible to use the Cholesky factorisation with complex sparse matrices.\n"), "eigs"));
        else
            if(issparse(%_B))
                [R,P] = spchol(%_B);
            else
                R = chol(%_B);
            end
            Rprime = R';
        end
    end

    //Main
    howmny = 'A';
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
                if(matB == 0)
                    ierr = execstr('A_fun(workd(ipntr(1):ipntr(1)+nA-1))', 'errcatch');
                    if(ierr <> 0)
                        break;
                    end
                    workd(ipntr(2):ipntr(2)+nA-1) = A_fun(workd(ipntr(1):ipntr(1)+nA-1));
                else
                    ierr = execstr('A_fun(inv(R) * workd(ipntr(1):ipntr(1)+nA-1))', 'errcatch');
                    if(ierr <> 0)
                        break;
                    end
                    workd(ipntr(2):ipntr(2)+nA-1) = inv(Rprime) * A_fun(inv(R) * workd(ipntr(1):ipntr(1)+nA-1));
                end
            elseif(iparam(7) == 3)
                if(matB == 0)
                    if(ido == 2)
                        workd(ipntr(2):ipntr(2)+nA-1) = workd(ipntr(1):ipntr(1)+nA-1);
                    else
                        ierr = execstr('A_fun(workd(ipntr(1):ipntr(1)+nA-1))', 'errcatch');
                        if(ierr <> 0)
                            break;
                        end
                        workd(ipntr(2):ipntr(2)+nA-1) = A_fun(workd(ipntr(1):ipntr(1)+nA-1));
                    end
                else
                    if(ido == 2)
                        if(cholB)
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime * R * workd(ipntr(1):ipntr(1)+nA-1);
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = %_B * workd(ipntr(1):ipntr(1)+nA-1);
                        end
                    elseif(ido == -1)
                        if(cholB)
                            workd(ipntr(2):ipntr(2)+nA-1) = Rprime * R * workd(ipntr(1):ipntr(1)+nA-1);
                        else
                            workd(ipntr(2):ipntr(2)+nA-1) = %_B * workd(ipntr(1):ipntr(1)+nA-1);
                        end
                        ierr = execstr('A_fun(workd(ipntr(2):ipntr(2)+nA-1))', 'errcatch');
                        if(ierr <> 0)
                            break;
                        end
                        workd(ipntr(2):ipntr(2)+nA-1) = A_fun(workd(ipntr(2):ipntr(2)+nA-1));
                    else
                        ierr = execstr('A_fun(workd(ipntr(3):ipntr(3)+nA-1))', 'errcatch');
                        if(ierr <> 0)
                            break;
                        end
                        workd(ipntr(2):ipntr(2)+nA-1) = A_fun(workd(ipntr(3):ipntr(3)+nA-1));
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
            [dr, di, z, resid, v, iparam, ipntr, workd, workl, info_eupd] = dneupd(rvec, howmny, _select, dr, di, z, sigmar, sigmai, workev, bmat, nA, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info);
            if(info <> 0)
                error(msprintf(gettext("%s: Error with %s: info = %d.\n"), "eigs", "DNEUPD", info));
            else
                res_d = complex(dr,di);
                res_d(nev+1) = [];
                if(rvec)
                    res_d = diag(res_d)
                    res_v = z;
                    c1 = 1:2:nev + 1;
                    c2 = 2:2:nev + 1;
                    if(modulo(nev,2) == 1)
                        c1($) = [];
                    end
                    res_v(:,[c1, c2]) = [res_v(:,c1) + res_v(:,c2) * %i res_v(:,c1) - res_v(:,c2) * %i];
                    res_v(:,$) = [];
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
endfunction
