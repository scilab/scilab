// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = %hmS_k_hmS_generic(a, b)
    
    // Code used by %s_k_hm(), %hm_k_s() and %hm_k_hm()
    // Fully rewritten and factorized after bug http://bugzilla.scilab.org/13339

    sa = size(a)
    sb = size(b)
    sa = [sa ones(1,ndims(b)-ndims(a))]
    sb = [sb ones(1,ndims(a)-ndims(b))]
    La = length(a)
    Lb = length(b)
    a = a(:)
    b = b(:)
    ia = (1:La).' .*.ones(b);
    ib = ones(a) .*. (1:Lb).';
    ir = (ia-1).*Lb + ib;
    pa = ind2sub(sa, ia)
    pb = ind2sub(sb, ib)
    clear ia ib
    pr = (pa-1).*repmat(sb, La*Lb, 1) + pb
    clear pa pb
    nir = sub2ind(sa.*sb, pr)
    [v,k] = gsort(nir,"g","i")
    clear pr nir v
    r = a.*.b
    r = matrix(r(ir(k)), sa.*sb)
endfunction
