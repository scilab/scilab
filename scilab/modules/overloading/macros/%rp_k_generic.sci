// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function res = %rp_k_generic(a,b)
    // a .*. b, with
    // a: polynomial or rational
    // b: number, polynomial, or rational

    ia = matrix(1:size(a,"*"), size(a));
    ib = matrix(1:size(b,"*"), size(b));

    // Size of the result:
    sa = size(a);
    sb = size(b);
    ndims_a = length(sa)
    ndims_b = length(sb)
    ndims_r = max(ndims_a, ndims_b);
    sa = [sa ones(1,(ndims_r-ndims_a))];
    sb = [sb ones(1,(ndims_r-ndims_b))];
    sr = sa .* sb;

    // PROCESSING
    // ----------
    if  typeof(a)~="rational" & typeof(b)~="rational"
        res = a(ia .*. ones(b))(:) .* b(ones(a) .*. ib)(:);
    else
        // Special case of rationals ( "(i)" indexing fails)
        //  => We apply kron() separately on the numerator and denominator
        if typeof(a)=="rational"
            anum = a.num
            aden = a.den
            dt = a.dt
        else
            anum = a
            aden = ones(a)
        end
        if typeof(b)=="rational"
            bnum = b.num
            bden = b.den
            dt = b.dt
        else
            bnum = b
            bden = ones(b)
        end
        ria = (ia.*.ones(b));     // replicated indices in a
        rib = (ones(a).*.ib);     // replicated indices in b
        //
        res = rlist(anum(ria)(:) .* bnum(rib)(:), aden(ria)(:) .* bden(rib)(:), dt);
    end

    // Final formatting
    res = matrix(res, sr);
endfunction
