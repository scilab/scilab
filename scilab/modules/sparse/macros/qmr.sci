// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) 2005 - IRISA - Sage Group

//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


// [x, flag, err, iter, res] = qmr( A, Ap, b, x, M1, M1p, M2, M2p, max_it, tol )
//
// QMR solves the linear system Ax=b using the
// Quasi Minimal Residual method with preconditioning.
//
// input   A        REAL matrix or function
//         x        REAL initial guess vector
//         b        REAL right hand side vector
//         M1       REAL left preconditioner matrix
//         M2       REAL right preconditioner matrix
//         max_it   INTEGER maximum number of iterations
//         tol      REAL error tolerance
//
// output  x        REAL solution vector
//         flag     INTEGER: 0: solution found to tolerance
//                           1: no convergence given max_it
//                     breakdown:
//                          -1: rho
//                          -2: Beta
//                          -3: gam
//                          -4: delta
//                          -5: ep
//                          -6: xi
//         err      REAL final residual norm
//         iter     INTEGER number of iterations performed
//         res      REAL residual vector

//     Details of this algorithm are described in
//
//     "Templates for the Solution of Linear Systems: Building Blocks
//     for Iterative Methods",
//     Barrett, Berry, Chan, Demmel, Donato, Dongarra, Eijkhout,
//     Pozo, Romine, and Van der Vorst, SIAM Publications, 1993
//     (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps).
//
//     "Iterative Methods for Sparse Linear Systems, Second Edition"
//     Saad, SIAM Publications, 2003
//     (ftp ftp.cs.umn.edu; cd dept/users/saad/PS; get all_ps.zip).

// Sage Group (IRISA, 2005)

function [x, flag, err, iter, res] = qmr( A, varargin)

    // -----------------------
    // Parsing input arguments
    // -----------------------
    [lhs,rhs]=argn(0);
    if ( rhs < 2 ),
        error(msprintf(gettext("%s: Wrong number of input arguments: At least %d expected.\n"),"qmr",2));
    end

    // Parsing the matrix A
    select type(A)
    case 1 then
        cpt=1;
    case 5 then
        cpt=1;
    case 13 then
        cpt=0;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d : A real or complex matrix or a sparse matrix or a function expected.\n"),"qmr",1));
    end

    // If A is a matrix (dense or sparse)
    if (cpt==1),
        if (size(A,1) ~= size(A,2)),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"qmr",1));
        end
        fct=0;
        cptmat = 1;
    end

    // If A is a function
    if (cpt==0),
        fct = 0;
        if rhs >= 2,
            funcorvec=varargin(1);
            if and(type(funcorvec) <> [1 5 13]) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Transpose of the function %s expected.\n"),"qmr",2,"A"));
                // if the following input argument is a sparse or dense matrix
            elseif or(type(funcorvec) == [1 5]) then
                if size(getfield(1,macrovar(A)),"*") == 1 then
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: A transpose of the function %s expected.\n"), "qmr", 2, "A"));
                end
                matvec = A;
                cptmat = 1;
                // if the following input argument is a function
            else
                matvec = A;
                matvecp = funcorvec;
                cptmat = 2;
                fct = 1;
                warning(msprintf(gettext("%s : Calling qmr(A,Ap) is deprecated. Please see qmr documentation for more details.\n"),"qmr"));
            end
        end
    end

    // Parsing right hand side b
    if ( rhs >= fct+2 ),
        b=varargin(fct+1);
        // if b is not constant or sparse
        if and(type(b) <> [1 5])  then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A real or complex, full or sparse column vector expected.\n"), "qmr", fct+2));
        end
        if ( size(b,2) ~= 1),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Column vector expected.\n"),"qmr",fct+2));
        end
    end

    // Parsing initial vector x
    if ( rhs >= fct+3),
        x=varargin(fct+2);
        // if x is not constant or sparse
        if and(type(x) <> [1 5])  then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A real or complex, full or sparse column vector expected.\n"), "qmr", fct + 3));
        end
        if (size(x,2) ~= 1),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Column vector expected.\n"),"qmr",fct+3));
        end
        if ( size(x,1) ~= size(b,1)),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"qmr",fct+3,fct+2));
        end
        // By default
    else
        x=zeros(size(b,1),1);
    end

    //--------------------------------------------------------
    // Parsing of the preconditioner matrix M1
    //--------------------------------------------------------
    if (rhs >=fct+4),
        Prec_g=varargin(fct+3);
        select type(Prec_g)
        case 1 then
            cpt=1;
        case 5 then
            cpt=1;
        case 13 then
            cpt=0;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A real or complex, full or sparse, square matrix or a function expected.\n"), "qmr", fct + 4));
        end

        // if M1 is a matrix
        if ( cpt==1 ),
            if (size(Prec_g,1) ~= size(Prec_g,2)),
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"qmr",fct+4));
            end
            if (size(Prec_g,1)~=size(b,1)),
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"qmr",fct+4,fct+2));
            end
            cptmatM1 = 1;
        end

        // if M1 is a function
        if ( cpt==0 ),
            precond_g = Prec_g;
            cptmatM1 = 1;
            if ( rhs >= fct+5 & size(getfield(1,macrovar(precond_g)),"*") == 1),
                Precp_g = varargin(fct+4);
                if (type(Precp_g) == 13 & size(getfield(1,macrovar(Precp_g)),"*")==1) then
                    //precond_g = Prec_g;
                    precondp_g = Precp_g;
                    cptmatM1 = 2;
                    fct = fct+1;
                    warning(msprintf(gettext("%s : Calling qmr(...,M1,M1p) is deprecated. Please see qmr documentation for more details.\n"),"qmr"));
                else
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: A transpose of the function expected.\n"), "qmr", fct + 5));
                end
            elseif rhs < fct+5 & size(getfield(1, macrovar(precond_g)), "*") == 1 then
                error(msprintf(gettext("%s: Wrong prototype of input argument #%d: If M1 is function, use the header M1(x,t) instead M1(x).\n"), "qmr", fct+4));
            end
        end
        // By default
    else
        deff("y=precond_g(x)","y=x");
        deff("y=precondp_g(x)","y=x");
        cptmatM1 = 2;
    end

    //--------------------------------------------------------
    // Parsing of the preconditioner matrix M2
    //--------------------------------------------------------
    if (rhs >=fct+5),
        Prec_d=varargin(fct+4);
        select type(Prec_d)
        case 1 then
            cpt=1;
        case 5 then
            cpt=1;
        case 13 then
            cpt=0;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A real or complex, full or sparse, square matrix or a function expected.\n"), "qmr", fct + 5));
        end

        // M2 matrix
        if ( cpt==1 ),

            if (size(Prec_d,1) ~= size(Prec_d,2)),
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"qmr",fct+5));
            end
            if (size(Prec_d,1)~=size(b,1)),
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"qmr",fct+5,fct+2));
            end
            cptmatM2 = 1;

        end

        // M2 function
        if ( cpt==0 )
            precond_d = Prec_d;
            cptmatM2 = 1;
            if ( rhs >= fct+6 & size(getfield(1,macrovar(precond_d)),"*") == 1),
                Precp_d=varargin(fct+5);
                if (type(Precp_d) == 13 & size(getfield(1,macrovar(Precp_d)),"*") == 1) then
                    precond_d = Prec_d;
                    precondp_d = Precp_d;
                    cptmatM2 = 2;
                    fct = fct+1;
                    warning(msprintf(gettext("%s : Calling qmr(...,M2,M2p) is deprecated. Please see qmr documentation for more details.\n"),"qmr"));
                else
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: A transpose of the function expected.\n"), "qmr", fct + 6));
                end
            elseif rhs < fct+6 & size(getfield(1, macrovar(precond_d)), "*") == 1 then
                error(msprintf(gettext("%s: Wrong prototype of input argument #%d: If M2 is function, use the header M2(x,t) instead M2(x).\n"), "qmr", fct+5));
            end
        end
        // By default
    else
        deff("y=precond_d(x)","y=x");
        deff("y=precondp_d(x)","y=x");
        cptmatM2 = 2;
    end

    //--------------------------------------------------------
    // Parsing of the maximum number of iterations max_it
    //--------------------------------------------------------
    if (rhs >= fct+6),
        max_it=varargin(fct+5);
        // if max_it is not constant
        if type(max_it) <> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"),"qmr",fct + 6));
        end

        if ~isreal(max_it) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A real scalar expected.\n"),"qmr",fct + 6));
        end

        if (size(max_it,1) ~= 1 | size(max_it,2) ~=1),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"),"qmr",fct+6));
        end
        // By default
    else
        max_it=size(b,1);
    end

    //--------------------------------------------------------
    // Parsing of the error tolerance tol
    //--------------------------------------------------------
    if (rhs == fct+7),
        tol=varargin(fct+6);
        // if tol is not constant
        if type(tol) <> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"),"qmr",fct + 7));
        end

        if ~isreal(tol) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A real scalar expected.\n"),"qmr",fct + 7));
        end

        if (size(tol,1) ~= 1 | size(tol,2) ~=1),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"),"qmr",fct+7));
        end
        // By default
    else
        tol=1000*%eps;
    end

    //--------------------------------------------------------
    // test about input arguments number
    //--------------------------------------------------------
    if (rhs > fct+7),
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"qmr",2,fct+7));
    end

    // ------------
    // Computations
    // ------------

    // initialization
    i = 0;
    flag = 0;
    iter = 0;
    bnrm2 = norm( b );
    if  (bnrm2 == 0.0),
        bnrm2 = 1.0;
    end

    //   r = b - A*x;
    if (cptmat == 1) then
        r = b - matvec(x,"notransp");
    elseif (cptmat==2) then  // If A is a function
        r = b - matvec(x);
    end

    err = norm( r ) / bnrm2;
    res = err;
    if ( err < tol ), return; end

    // [M1,M2] = lu( M );
    v_tld = r;

    // y = M1 \ v_tld;
    if (cptmatM1 == 1) then
        y = precond_g(v_tld,"notransp");
    elseif (cptmatM1==2) then
        y = precond_g(v_tld);
    end

    rho = norm( y );

    w_tld = r;
    //   z = M2' \ w_tld;
    if (cptmatM2 == 1) then
        z = precond_d(w_tld,"transp");
    elseif (cptmatM2 == 2) then
        z = precondp_d(w_tld);
    end

    xi = norm( z );

    gam =  1.0;
    eta = -1.0;
    theta =  0.0;

    for i = 1:max_it,                      // begin iteration
        if ( rho == 0.0 | xi == 0.0 ), iter=i; break; end
        v = v_tld / rho;
        y = y / rho;

        w = w_tld / xi;
        z = z / xi;

        delta = z'*y;
        if ( delta == 0.0 ), iter=i; break; end

        //    y_tld = M2 \ y;
        if (cptmatM2 == 1) then
            y_tld = precond_d(y,"notransp");
        elseif (cptmatM2 == 2) then
            y_tld = precond_d(y);
        end

        //    z_tld = M1'\ z;
        if (cptmatM1 == 1) then
            z_tld = precond_g(z,"transp");
        elseif (cptmatM1 == 2) then
            z_tld = precondp_g(z);
        end

        if ( i > 1 ),                       // direction vector
            p = y_tld - ( xi*delta / ep )*p;
            q = z_tld - ( rho*delta / ep )*q;
        else
            p = y_tld;
            q = z_tld;
        end

        //    p_tld = A*p;
        if (cptmat == 1) then
            p_tld = matvec(p,"notransp");
        elseif (cptmat == 2) then
            p_tld = matvec(p);
        end

        ep = q'*p_tld;
        if ( ep == 0.0 ), iter=i; break; end

        Beta = ep / delta;
        if ( Beta == 0.0 ), iter=i; break; end

        v_tld = p_tld - Beta*v;

        //    y =  M1 \ v_tld;
        if (cptmatM1 == 1) then
            y = precond_g(v_tld,"notransp");
        elseif (cptmatM1==2) then
            y = precond_g(v_tld);
        end

        rho_1 = rho;
        rho = norm( y );

        //    w_tld = ( A'*q ) - ( Beta*w );
        if (cptmat == 1) then
            w_tld = ( matvec(q,"transp") ) - ( Beta*w );
        elseif (cptmat == 2) then
            w_tld = ( matvecp(q) ) - ( Beta*w );
        end

        //    z =  M2' \ w_tld;
        if (cptmatM2 == 1) then
            z = precond_d(w_tld,"transp");
        elseif (cptmatM2 == 2) then
            z = precondp_d(w_tld);
        end

        xi = norm( z );
        gamma_1 = gam;
        theta_1 = theta;
        theta = rho / ( gamma_1*Beta );
        gam = 1.0 / sqrt( 1.0 + (theta^2) );
        if ( gam == 0.0 ), iter=i; break; end

        eta = -eta*rho_1*(gam^2) / ( Beta*(gamma_1^2) );

        if ( i > 1 ),                         // compute adjustment
            d = eta*p + (( theta_1*gam )^2)*d;
            s = eta*p_tld + (( theta_1*gam )^2)*s;
        else
            d = eta*p;
            s = eta*p_tld;
        end
        x = x + d;                               // update approximation

        r = r - s;                               // update residual
        err = norm( r ) / bnrm2;               // check convergence
        res = [res;err];

        if ( err <= tol ), iter=i; break; end

        if ( i == max_it ), iter=i; end
    end

    if ( err <= tol ),                        // converged
        flag =  0;
    elseif ( rho == 0.0 ),                      // breakdown
        flag = -1;
    elseif ( Beta == 0.0 ),
        flag = -2;
    elseif ( gam == 0.0 ),
        flag = -3;
    elseif ( delta == 0.0 ),
        flag = -4;
    elseif ( ep == 0.0 ),
        flag = -5;
    elseif ( xi == 0.0 ),
        flag = -6;
    else                                        // no convergence
        flag = 1;
    end

endfunction

function y = matvec(x,t)
    if (t=="notransp") then
        y = A*x;
    elseif (t=="transp") then
        y = A'*x;
    end
endfunction

function y = precond_g(x,t)
    if (t=="notransp") then
        y = Prec_g*x;
    elseif (t=="transp") then
        y = Prec_g'*x;
    end
endfunction

function y = precond_d(x,t)
    if (t=="notransp") then
        y = Prec_d*x;
    elseif (t=="transp") then
        y = Prec_d'*x;
    end
endfunction

