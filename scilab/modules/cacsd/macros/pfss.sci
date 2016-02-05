// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) - 2014 - Samuel GOUGEON <sgougeon@free.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function elts = pfss(S, rmax, cord)
    //Syntax : elts = pfss(S)
    //Partial fraction decomposition of the linear system S (in state-space form):
    // elts is the list of linear systems which add up to S
    // i.e. elts = list(S1, S2, S3, ..., Sn) with S1 + S2 +... +Sn = S
    // Each Si contains some poles of S according to the block-diagonalization
    // of the A matrix of S.
    // If S is given in transfer form, it is first converted into state-space
    // and each subsystem is then converted in transfer form.

    select argn(2)
    case 1 then
        rmax = [];
        cord = [];
    case 2 then
        if type(rmax) == 10 then
            cord = rmax;
            rmax=[];
        elseif type(rmax) == 1 then
            cord = [];
        end
    end

    if and(typeof(S) <> ["rational", "state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"), "pfss", 1))
    end
    if typeof(S) == "rational" then
        flag = varn(S("num"))
        S = tf2ss(S)
    else
        flag = ""
    end

    [f, g, h, dd, dom] = S([2:5, 7]);
    [n, n] = size(f);
    if rmax == [] then
        [f, x, bs] = bdiag(f,max(100,norm(f,1)));
    else
        [f, x, bs] = bdiag(f, rmax);
    end
    h = h*x; g = x\g;
    k = 1; ll = 0;
    elts = list();
    for l = bs',
        ind = k:k+l-1;
        f1l = f(ind, ind);
        gl = g(ind, :);
        hl = h(:, ind);
        elts(ll+1) = syslin("c", f1l, gl, hl)
        ll = ll+1; k = k+l;
    end;
    if argn(2) == 2  then
        select cord
        case "c"
            nb = size(bs, "*");
            class = [];
            for k = 1:nb
                oneortwo = bs(k); ss = elts(k); A = ss(2);
                if oneortwo == 1 then
                    class = [class, real(spec(A))];
                end
                if oneortwo > 1 then
                    class = [class, min(real(spec(A)))];
                end
            end;
            [cl, indi] = gsort(-class);
            elts1 = elts;
            for k = 1:size(elts);
                elts(k) = elts1(indi(k));
            end
        case "d"
            nb = size(bs, "*");
            class = [];
            for k = 1:nb
                oneortwo = bs(k); ss = elts(k); A = ss(2);
                if oneortwo == 1 then
                    class = [class, abs(spec(A))];
                end
                if oneortwo > 1 then
                    class = [class, max(abs(spec(A)))];
                end
            end;
            [cl, indi] = gsort(-class);
            elts1 = elts;
            for k = 1:size(elts);
                elts(k) = elts1(indi(k));
            end
        end
    end
    if type(dd) == 1 then
        if norm(dd, "fro") <> 0 then elts(ll+1) = dd, end
    end
    if type(dd) == 2 then
        if norm(coeff(dd), 1) > %eps then elts(ll+1) = dd, end
    end
    if flag ~= "" then
        k = size(elts)
        for kk = 1:k
            elts(kk) = varn(ss2tf(elts(kk)), flag)
        end
    end
endfunction
