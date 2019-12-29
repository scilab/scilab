// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Samuel GOUGEON - Le Mans Université
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = ellipj(varargin)
    //           sn = ellipj(x, m)  // Replaces %sn(x,m)
    //     [sn, cn] = ellipj(x, m)  // Returns the cn function in addition to sn
    // [sn, cn, dn] = ellipj(x, m)  // Returns dn function in addition to sn and cn
    //           st = ellipj(funNames, x, m)
    //               // Returns the Jacobi functions named in the funNames
    //               //  vector of strings among "sn" "cn" "dn" "ns" "nc" "nd".
    //               //  Values are returned through the st structure whose fields
    //               //  are the elements of funNames.
    //               //  Later, this syntax could be easily extendable to any
    //               //  of the other non trivial "cd" "cs" "dc" "ds" "sc"
    //               //  and "sd" Jacobi functions.

    fname = "ellipj"
    lhs = argn(1)
    funs = []   // names of Jacobi functions to be returned
    shift = 0   // input argument shift, for error messages about x or m

    // CHECKING ARGUMENTS
    // ==================
    in = varargin
    sizin = length(varargin)
    if sizin < 2 | sizin > 3 then
        msg = _("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, fname, 2, 3))
    end
    if sizin==3 then
        [funs, k]= unique(in(1))
        funs = in(1)(gsort(k,"g","i"))
        in(1) = null()
        if lhs > 1
            msg = _("%s: Wrong number of output arguments: %d expected.\n")
            error(msprintf(msg, fname, 1))
        end
        shift = 1
        if type(funs) <> 10
            msg = _("%s: Argument #%d: Text expected.\n")
            error(msprintf(msg, fname, 1))
        end
        if length(grep(["cn" "dn" "sn" "nc" "nd" "ns"], funs)) <> size(funs,"*")
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, fname, 1, "''cn'' ''dn'' ''sn'' ''nc'' ''nd'' ''ns''"))
        end
    else
        if lhs > 3
            msg = _("%s: Wrong number of output arguments: %d to %d expected.\n")
            error(msprintf(msg, fname, 1, 3))
        end
    end
    // x
    x = in(1)
    if type(x) <> 1 then
        msg = _("%s: Argument #%d: Decimal or complex number expected.\n")
        error(msprintf(msg, fname, 1+shift))
    end
    sx = size(x)
    x = x(:)
    // m
    m = in(2)
    if type(m) <> 1 | ~isreal(m,0) then
        msg = _("%s: Argument #%d: Decimal number expected.\n")
        error(msprintf(msg, fname, 2+shift))
    end
    if or(m < 0) | or(m > 1) then
        msg = _("%s: Argument #%d: Must be in the interval [%d, %d].\n")
        error(msprintf(msg, fname, 2+shift, 0, 1))
    end

    // PROCESSING
    // ==========
    [n1,n2] = size(x);
    n = n1*n2;
    a = amell(real(x), sqrt(m));
    s = sin(a);
    c = cos(a);
    d = sqrt(ones(n1,n2) - m*s.*s);
    xReal = isreal(x,0)
    if ~xReal then
        m1 = 1 - m;
        a1 = amell(imag(x), sqrt(m1));
        s1 = sin(a1);
        c1 = cos(a1);
        d1 = sqrt(ones(n1,n2) - m1*s1.*s1);
        N = (c1.*c1 + m*s.*s.*s1.*s1);
    end
    kinf = isinf(x)

    // sn
    // --
    if funs==[] | grep(funs,["sn" "ns" "dn" "nd"]) <> [] then
        if xReal
            sn = s
        else
            sn = (s.*d1 + %i*c.*d.*s1.*c1) ./ N;
        end
        if ~isreal(sn) & isreal(sn,0) then
            sn = real(sn)
        end
        sn(kinf) = %nan
        sn = matrix(sn, sx)
    end
    // cn
    // --
    if lhs > 1 | grep(funs,["cn" "nc"]) <> [] then
        if xReal
            cn = c
        else
            cn = (c.*c1 - %i*s.*d.*s1.*d1) ./ N;
        end
        if ~isreal(cn) & isreal(cn,0) then
            cn = real(cn)
        end
        cn(kinf) = %nan
        cn = matrix(cn, sx)
    end
    // dn
    // --
    if lhs > 2 | grep(funs,["dn" "nd"]) <> [] then
        dn = sqrt(1 - m*sn.*sn);
        if ~isreal(dn) & isreal(dn,0) then
            dn = real(dn)
        end
    end
    // nc, nd, ns
    // ----------
    if funs <> [] then
        if grep(funs, "nc") <> [], nc = 1 ./ cn, end
        if grep(funs, "nd") <> [], nd = 1 ./ dn, end
        if grep(funs, "ns") <> [], ns = 1 ./ sn, end
    end

    // OUTPUT
    // ======
    if funs <> [] then
        st = struct()
        for f = funs(:)'
            execstr("st(f) = "+f)
        end
        varargout = list(st)
    else
        varargout = list()
        varargout(1) = sn
        if lhs > 1, varargout(2) = cn, end
        if lhs > 2, varargout(3) = dn, end
    end
endfunction
