// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) - 2012
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [n,d]=%p_simp(num,den)
    // implement complex case
    // Check the variable name
    var_name_num = varn(num);
    var_name_den = varn(den);

    if var_name_num <> var_name_den then
        error(msprintf(gettext("%s: Wrong value for input argument #%d and #%d: Variable name of numerator and denominator must be the same.\n"), "simp", 1, 2));
    end

    [m1, m2] = size(num);
    n = zeros(m1, m2);
    d = n;

    for k = 1:m1
        for l = 1:m2
            tmp_n = num(k, l)
            tmp_d = den(k, l)
            if imag(tmp_n) == 0 & imag(tmp_d) == 0 then
                [tmp_n, tmp_d] = simp(real(tmp_n), real(tmp_d))
            else
                rn = roots(tmp_n);
                rd = roots(tmp_d);
                if length(rd) > length(rn) then
                    r = rn;
                else
                    r = rd
                end
                for i = 1:length(r)
                    if abs(horner(tmp_n, r(i))) < sqrt(%eps) then
                        pol = poly(r(i), var_name_num, "roots");
                        tmp_n = pdiv(tmp_n, pol);
                        tmp_d = pdiv(tmp_d, pol);
                        if real(tmp_n) == 0 & real(tmp_d) == 0 then
                            tmp_n = imag(tmp_n);
                            tmp_d = imag(tmp_d);
                        end
                    end
                end
            end
            n(k,l) = tmp_n
            d(k,l) = tmp_d
        end
    end
endfunction
