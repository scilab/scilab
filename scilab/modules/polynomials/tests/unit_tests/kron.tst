// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// a .*. b with a and b number, polynomial, or rational (but not both numbers)

// Numbers
sn = 2;             // scalar
rn = [2 3];         // row
cn = [-1 2]';       // column
mn = cn*rn;         // matrix
hn = cat(3,mn, mn');// hypermatrix
ln = list(sn,rn,cn,mn,hn);

// Polynomials
sp = 1+%z^2;
rp = [1+%z 1-%z^2];
cp = [1+%z^3 ; 2-%z];
mp = cp*rp;
hp = cat(4,mp, mp');
lp = list(sp,rp,cp,mp,hp);

// Rationals
sr = 1/%z;
rr = [1/%z %z/(1-%z)];
cr = [(1-%z)/%z ; 1+%z];
mr = cr*rr;
hr = cat(4, mr, mr');
lr = list(sr,rr,cr,mr,hr);

// TESTS of executability and of the size of the result
// ----------------------------------------------------
// Size of the result:
function s = sizeOfKron(sa, sb)
    ndims_a = length(sa);
    ndims_b = length(sb);
    ndims_r = max(ndims_a, ndims_b);
    sa = [sa ones(1,(ndims_r-ndims_a))];
    sb = [sb ones(1,(ndims_r-ndims_b))];
    s = sa .* sb;
endfunction

for p = lp
    for r = lr
        res = p .*. r;
        assert_checkequal(size(res), sizeOfKron(size(p), size(r)));
        res = r .*. p;
        assert_checkequal(size(res), sizeOfKron(size(r), size(p)));
    end
    for n = ln
        res = p .*. n;
        assert_checkequal(size(res), sizeOfKron(size(p), size(n)));
        res = n .*. p;
        assert_checkequal(size(res), sizeOfKron(size(n), size(p)));
    end
end
for r = lr
    for n = ln
        res = r .*. n;
        assert_checkequal(size(res), sizeOfKron(size(r), size(n)));
        res = n .*. r;
        assert_checkequal(size(res), sizeOfKron(size(n), size(r)));
    end
end


