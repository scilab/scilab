// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
