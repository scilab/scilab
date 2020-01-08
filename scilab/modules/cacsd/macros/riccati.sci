// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x1, x2, residual] = riccati(a, b, c, dom, method)
    // x = riccati(h)               // continuous time domain
    // x = riccati(h, e)            // discrete time domain
    // x = riccati(a, b, c, dom)
    // x = riccati(a, b, c, dom, method)
    //[x1,x2,residual] = riccati(..)  such that x = x1/x2 solve :
    //  a'*x + x*a - x*b*x + c = 0 (continuous time case)
    //  a'*x*a - (a'*x*b1/(b2+b1'*x*b1)) * (b1'*x*a) + c - x = 0 (discrete time case)
    //     with b = b1 / b2 * b1'
    //
    // -- a,b,c real matrices of size n x n
    //    For the discrete time domain: b and c must be symmetric.
    // -- h, e real matrices of size 2n x 2n
    // -- dom = 'c' or 'd' for the time domain (continuous or discrete)
    // -- method = 'eigen' --->block diagonalization
    //    method = 'schur' --->schur method

    [x1, x2, residual] = ([], [], [])
    [lhs,rhs] = argn(0)
    if ~or(rhs==[1 2 4 5]) then
        msg = _("%s: Wrong number of input arguments: %s expected.\n")
        error(msprintf(msg, "riccati", "1,2,4 or 5"))
    end
    msg = gettext("%s: No solution found. Dimension(stable subspace)=%d. %d expected.\n")

    if rhs > 2 then
        if ~isdef("method","l") then
            method = "eigen"
        end
        ham = [a, -b ; -c, -a']
        [n,n] = size(a)
        if part(dom,1)=="c" then    // riccati(a, b, c, "continuous")
            select  method,
            case "schur" then
                [s,d] = schur(ham, "c")
                if d <> n then
                    warning(msprintf(msg, "riccati", d, n))
                    return
                end
                s = s(:,1:n),
            case "eigen" then
                [hb,u1] = bdiag(ham),
                [u2,d] = schur(hb,"c")
                u = u1*u2,
                if d <> n then
                    warning(msprintf(msg, "riccati", d, n))
                    return
                end
                s = u(:,1:n)
            end

        else                        // riccati(a, b, c, "discrete")
            E = [eye(n,n), b ; zeros(n,n), a']
            H = [a, zeros(n,n) ; -c, eye(n,n)]
            [bs,as,s,d] = schur(H, E, "d");
            if d <> n then
                warning(msprintf(msg, "riccati", d, n))
                return
            end
            s = s(:,1:n);
        end
        x1 = s(n+1:2*n,:)
        x2 = s(1:n,:),
        if lhs > 2
            x = x1 / x2
            if part(dom,1)=="c"
                residual = norm(a'*x + x*a -x*b*x + c,1)
            else
                b1 = real(sqrtm(b))
                b2 = eye(b)
                if norm(b1/b2*b1'-b) / norm(b) > 1e3*%eps
                    // This can occur when b is not symetric
                    // Then sqrtm(b) may be highly complex
                    b1 = eye(b)
                    try
                        b2 = inv(b) // b may not be invertible
                    catch
                        b2 = %nan
                    end
                end
                // A'*X*A - (A'*X*B1) / (B2 + B1'*X*B1) * (B1'*X*A) + C - X = 0
                residual = norm(a'*x*a - (a'*x*b1 / (b2+b1'*x*b1)) * (b1'*x*a)+c-x,1)
            end
        end

    else   // from the Hamiltonian H and the Hamiltonian pencil (E,H)
        H = a
        n2 = size(H,2)
        n1 = n2/2
        if rhs==1 then  // riccati(H): Continuous
            A = H(1:n1,1:n1);
            [Hb,W1] = bdiag(H);
            if cond(W1) > 1.d10*norm(H,1) then
                // write(%io(2),'Warning : Bad conditioning => balancing');
                [Hb,W1] = balanc(H);
            end
            if cond(W1) > 1.d+10*norm(H,1) then
                Hb = H
                W1 = eye(W1)
            end
            [W2,n] = schur(Hb,"c")
            if n <> n1 then
                warning(msprintf(msg, "riccati", n, n1))
            else
                W1 = W1*W2
                UV = W1(:,1:n1)
                x2 = UV(1:n1,:)
                x1 = UV(n1+1:n2,:)
                if lhs > 2
                    x = x1 / x2
                    // H = [A, -b ; -c, -A']
                    b = -H(1:n1,n1+1:$)
                    c = -H(n1+1:$,1:n1)
                    residual = norm(A'*x + x*A - x*b*x + c, 1)
                end
            end

        else  // riccati(H, E): Discrete
            E = b
            [UV,n] = schur(H,E,"d")
            if n <> n1 then
                warning(msprintf(msg, "riccati", n, n1))
            else
                x2 = UV(1:n,1:n)
                x1 = UV(n+1:2*n,1:n)
                if lhs > 2 then
                    A = H(1:n,1:n)
                    B = E(1:n,n+1:2*n)
                    C = -H(n+1:2*n,1:n)
                    B1 = real(sqrtm(B))
                    B2 = eye(A)
                    if norm(B1/B2*B1'-B) / norm(B) > 1e3*%eps
                        // This can occur when B is not symetric
                        // Then sqrtm(B) may be complex with non residual imaginary part
                        B1 = eye(B)
                        try
                            B2 = inv(B)
                            // B may not be invertible => no way to assess residual
                        catch
                            B2 = %nan
                        end
                    end
                    X = x1 / x2
                    // A'*X*A - (A'*X*B1) / (B2 + B1'*X*B1) * (B1'*X*A) + C - X = 0
                    residual = norm(A'*X*A - (A'*X*B1/(B2+B1'*X*B1))*(B1'*X*A) + C - X, 1)
                end
            end
        end
    end
    if lhs==1 & x1 <> []
        x1 = x1 / x2
    end

endfunction
